// Copyright CryptoKombat. Phase 1 scaffold.

#include "Game/FightGameMode.h"
#include "Game/FightGameState.h"
#include "Characters/CryptoFighter.h"
#include "Camera/FightCamera.h"
#include "Arena/BlockchainColosseum.h"
#include "UI/FightHUD.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "CryptoKombat.h"

AFightGameMode::AFightGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	GameStateClass = AFightGameState::StaticClass();
	DefaultPawnClass = ACryptoFighter::StaticClass();
	HUDClass = AFightHUD::StaticClass();
	ArenaClass = ABlockchainColosseum::StaticClass();
}

void AFightGameMode::BeginPlay()
{
	Super::BeginPlay();
	StartMatchFlow();
}

void AFightGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	AFightGameState* GS = GetFightState();
	if (!GS || !bMatchStarted)
	{
		return;
	}

	UpdateFacing();

	if (PhaseTimer > 0.f)
	{
		PhaseTimer -= DeltaSeconds;
	}

	switch (GS->RoundPhase)
	{
	case ERoundPhase::PreRound:
		if (PhaseTimer <= 0.f)
		{
			GS->RoundPhase = ERoundPhase::Fighting;
			GS->ResetRoundClock();
			UE_LOG(LogCryptoKombat, Log, TEXT("FIGHT! Round %d"), GS->CurrentRound);
		}
		break;

	case ERoundPhase::Fighting:
		GS->TickRoundClock(DeltaSeconds);
		CheckForKO();
		if (GS->RoundTimeRemaining <= 0.f)
		{
			// Time over — higher health wins.
			const float H1 = P1Fighter ? P1Fighter->CurrentHealth : 0.f;
			const float H2 = P2Fighter ? P2Fighter->CurrentHealth : 0.f;
			if (H1 == H2)
			{
				UE_LOG(LogCryptoKombat, Log, TEXT("Round draw on time — sudden death not implemented; P1 edge."));
				EndRound(0);
			}
			else
			{
				EndRound(H1 > H2 ? 0 : 1);
			}
		}
		break;

	case ERoundPhase::RoundEnd:
		if (PhaseTimer <= 0.f)
		{
			if (GS->IsMatchOver())
			{
				GS->RoundPhase = ERoundPhase::MatchEnd;
				UE_LOG(LogCryptoKombat, Log, TEXT("MATCH OVER — P1 %d | P2 %d"), GS->P1RoundsWon, GS->P2RoundsWon);
			}
			else
			{
				GS->CurrentRound++;
				BeginRound();
			}
		}
		break;

	default:
		break;
	}

	if (FightCamera && P1Fighter && P2Fighter)
	{
		FightCamera->SetTrackedFighters(P1Fighter, P2Fighter);
	}
}

void AFightGameMode::StartMatchFlow()
{
	EnsureArena();
	SpawnFighters();
	SpawnCamera();
	bMatchStarted = true;
	if (AFightGameState* GS = GetFightState())
	{
		GS->P1RoundsWon = 0;
		GS->P2RoundsWon = 0;
		GS->CurrentRound = 1;
	}
	BeginRound();
}

void AFightGameMode::EnsureArena()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// Prefer an arena already placed in the level.
	for (TActorIterator<ABlockchainColosseum> It(World); It; ++It)
	{
		ActiveArena = *It;
		break;
	}

	if (!ActiveArena && bAutoSpawnArena)
	{
		UClass* ClassToSpawn = ArenaClass ? ArenaClass.Get() : ABlockchainColosseum::StaticClass();
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ActiveArena = World->SpawnActor<ABlockchainColosseum>(ClassToSpawn, FVector::ZeroVector, FRotator::ZeroRotator, Params);
		UE_LOG(LogCryptoKombat, Log, TEXT("Auto-spawned Blockchain Colosseum arena."));
	}

	if (ActiveArena)
	{
		P1SpawnLocation = ActiveArena->GetP1Spawn();
		P2SpawnLocation = ActiveArena->GetP2Spawn();
	}
}

void AFightGameMode::BeginRound()
{
	AFightGameState* GS = GetFightState();
	if (!GS)
	{
		return;
	}

	GS->RoundPhase = ERoundPhase::PreRound;
	GS->ResetRoundClock();
	PhaseTimer = PreRoundDelay;

	if (ActiveArena)
	{
		P1SpawnLocation = ActiveArena->GetP1Spawn();
		P2SpawnLocation = ActiveArena->GetP2Spawn();
	}

	if (P1Fighter)
	{
		P1Fighter->SetActorLocation(P1SpawnLocation);
		P1Fighter->ApplyFighterDefinition(FindRosterFighter(P1FighterId));
		P1Fighter->SetFacingRight(true);
	}
	if (P2Fighter)
	{
		P2Fighter->SetActorLocation(P2SpawnLocation);
		P2Fighter->ApplyFighterDefinition(FindRosterFighter(P2FighterId));
		P2Fighter->SetFacingRight(false);
	}

	UE_LOG(LogCryptoKombat, Log, TEXT("Round %d — Ready..."), GS->CurrentRound);
}

void AFightGameMode::EndRound(int32 WinningPlayerIndex)
{
	AFightGameState* GS = GetFightState();
	if (!GS || GS->RoundPhase != ERoundPhase::Fighting)
	{
		return;
	}

	if (WinningPlayerIndex == 0)
	{
		GS->P1RoundsWon++;
	}
	else
	{
		GS->P2RoundsWon++;
	}

	GS->RoundPhase = ERoundPhase::RoundEnd;
	PhaseTimer = RoundEndDelay;
	UE_LOG(LogCryptoKombat, Log, TEXT("Round over — Winner P%d (score %d-%d)"),
		WinningPlayerIndex + 1, GS->P1RoundsWon, GS->P2RoundsWon);
}

void AFightGameMode::CheckForKO()
{
	if (!P1Fighter || !P2Fighter)
	{
		return;
	}
	const bool bP1KO = P1Fighter->GetFighterState() == EFighterState::KO;
	const bool bP2KO = P2Fighter->GetFighterState() == EFighterState::KO;
	if (bP1KO && bP2KO)
	{
		EndRound(0); // double KO — P1 edge for Phase 1
	}
	else if (bP2KO)
	{
		EndRound(0);
	}
	else if (bP1KO)
	{
		EndRound(1);
	}
}

FFighterDefinition AFightGameMode::FindRosterFighter(FName FighterId) const
{
	const TArray<FFighterDefinition> Roster = FCryptoRosterFactory::MakeDefaultRoster();
	for (const FFighterDefinition& Def : Roster)
	{
		if (Def.FighterId == FighterId)
		{
			return Def;
		}
	}
	return FCryptoRosterFactory::MakeSatoshiShadow();
}

void AFightGameMode::SpawnFighters()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	UClass* ClassToSpawn = FighterClass ? FighterClass.Get() : ACryptoFighter::StaticClass();

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	P1Fighter = World->SpawnActor<ACryptoFighter>(ClassToSpawn, P1SpawnLocation, FRotator::ZeroRotator, Params);
	P2Fighter = World->SpawnActor<ACryptoFighter>(ClassToSpawn, P2SpawnLocation, FRotator(0.f, 180.f, 0.f), Params);

	if (P1Fighter)
	{
		P1Fighter->PlayerIndex = 0;
		P1Fighter->ApplyFighterDefinition(FindRosterFighter(P1FighterId));
		// Possess P1 with first player controller
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0))
		{
			PC->Possess(P1Fighter);
		}
	}
	if (P2Fighter)
	{
		P2Fighter->PlayerIndex = 1;
		P2Fighter->ApplyFighterDefinition(FindRosterFighter(P2FighterId));
		// Create / use second local player for same-machine versus
		FString Error;
		UGameplayStatics::CreatePlayer(World, 1, true);
		if (APlayerController* PC2 = UGameplayStatics::GetPlayerController(World, 1))
		{
			PC2->Possess(P2Fighter);
		}
	}
}

void AFightGameMode::SpawnCamera()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	UClass* CamClass = CameraClass ? CameraClass.Get() : AFightCamera::StaticClass();
	FActorSpawnParameters Params;
	FightCamera = World->SpawnActor<AFightCamera>(CamClass, FVector(-800.f, 0.f, 120.f), FRotator::ZeroRotator, Params);
	if (FightCamera)
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0))
		{
			PC->SetViewTarget(FightCamera);
		}
	}
}

void AFightGameMode::UpdateFacing()
{
	if (P1Fighter && P2Fighter)
	{
		P1Fighter->FaceOpponent(P2Fighter);
		P2Fighter->FaceOpponent(P1Fighter);
	}
}

AFightGameState* AFightGameMode::GetFightState() const
{
	return GetGameState<AFightGameState>();
}
