// Copyright CryptoKombat. Phase 1 scaffold.

#include "Characters/CryptoFighter.h"
#include "Characters/FighterVisuals.h"
#include "Data/FighterTypes.h"
#include "VFX/HitSpark.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputActionValue.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "CryptoKombat.h"

ACryptoFighter::ACryptoFighter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 450.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Constrain to a side-view plane (X = depth locked loosely by gameplay; Y = along stage).
	GetCharacterMovement()->SetPlaneConstraintEnabled(true);
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(1.f, 0.f, 0.f));
	GetCharacterMovement()->bConstrainToPlane = true;

	// Procedural block-fighter visuals under the capsule (collision unchanged).
	FighterVisuals = CreateDefaultSubobject<UFighterVisuals>(TEXT("FighterVisuals"));
	FighterVisuals->SetupAttachment(RootComponent);

	// Hide default skeletal mesh — we use BasicShapes body instead.
	if (USkeletalMeshComponent* Skel = GetMesh())
	{
		Skel->SetHiddenInGame(true);
		Skel->SetVisibility(false);
		Skel->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ACryptoFighter::BeginPlay()
{
	Super::BeginPlay();

	if (FighterData.FighterId.IsNone())
	{
		// Default seed so PIE works without a DataTable yet.
		ApplyFighterDefinition(FCryptoRosterFactory::MakeSatoshiShadow());
	}
	else
	{
		ApplyFighterDefinition(FighterData);
	}

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (MappingContext)
			{
				Subsystem->AddMappingContext(MappingContext, 0);
			}
		}
	}
}

void ACryptoFighter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickStateTimers(DeltaTime);

	// Passive Moon Meter drip while not KO.
	if (FighterState != EFighterState::KO)
	{
		AddMoonMeter(2.f * DeltaTime);
	}
}

void ACryptoFighter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACryptoFighter::HandleMove);
		}
		if (JumpAction)
		{
			EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &ACryptoFighter::HandleJump);
		}
		if (PunchLightAction)
		{
			EIC->BindAction(PunchLightAction, ETriggerEvent::Started, this, &ACryptoFighter::OnPunchLight);
		}
		if (PunchHeavyAction)
		{
			EIC->BindAction(PunchHeavyAction, ETriggerEvent::Started, this, &ACryptoFighter::OnPunchHeavy);
		}
		if (KickLightAction)
		{
			EIC->BindAction(KickLightAction, ETriggerEvent::Started, this, &ACryptoFighter::OnKickLight);
		}
		if (KickHeavyAction)
		{
			EIC->BindAction(KickHeavyAction, ETriggerEvent::Started, this, &ACryptoFighter::OnKickHeavy);
		}
		if (Special1Action)
		{
			EIC->BindAction(Special1Action, ETriggerEvent::Started, this, &ACryptoFighter::OnSpecial1);
		}
		if (Special2Action)
		{
			EIC->BindAction(Special2Action, ETriggerEvent::Started, this, &ACryptoFighter::OnSpecial2);
		}
		if (BlockAction)
		{
			EIC->BindAction(BlockAction, ETriggerEvent::Started, this, &ACryptoFighter::OnBlockPressed);
			EIC->BindAction(BlockAction, ETriggerEvent::Completed, this, &ACryptoFighter::OnBlockReleased);
		}
		return;
	}

	// Fallback classic bindings for same-keyboard versus without EI assets yet.
	// P1: WASD + J/K/L/U/I/O | P2: Arrows + Numpad
	if (PlayerIndex == 0)
	{
		PlayerInputComponent->BindAxis("P1_Move", this, &ACryptoFighter::MoveHorizontal);
		PlayerInputComponent->BindAction("P1_Jump", IE_Pressed, this, &ACryptoFighter::OnJumpPressed);
		PlayerInputComponent->BindAction("P1_PunchLight", IE_Pressed, this, &ACryptoFighter::OnPunchLight);
		PlayerInputComponent->BindAction("P1_PunchHeavy", IE_Pressed, this, &ACryptoFighter::OnPunchHeavy);
		PlayerInputComponent->BindAction("P1_KickLight", IE_Pressed, this, &ACryptoFighter::OnKickLight);
		PlayerInputComponent->BindAction("P1_KickHeavy", IE_Pressed, this, &ACryptoFighter::OnKickHeavy);
		PlayerInputComponent->BindAction("P1_Special1", IE_Pressed, this, &ACryptoFighter::OnSpecial1);
		PlayerInputComponent->BindAction("P1_Special2", IE_Pressed, this, &ACryptoFighter::OnSpecial2);
		PlayerInputComponent->BindAction("P1_Block", IE_Pressed, this, &ACryptoFighter::OnBlockPressed);
		PlayerInputComponent->BindAction("P1_Block", IE_Released, this, &ACryptoFighter::OnBlockReleased);
	}
	else
	{
		PlayerInputComponent->BindAxis("P2_Move", this, &ACryptoFighter::MoveHorizontal);
		PlayerInputComponent->BindAction("P2_Jump", IE_Pressed, this, &ACryptoFighter::OnJumpPressed);
		PlayerInputComponent->BindAction("P2_PunchLight", IE_Pressed, this, &ACryptoFighter::OnPunchLight);
		PlayerInputComponent->BindAction("P2_PunchHeavy", IE_Pressed, this, &ACryptoFighter::OnPunchHeavy);
		PlayerInputComponent->BindAction("P2_KickLight", IE_Pressed, this, &ACryptoFighter::OnKickLight);
		PlayerInputComponent->BindAction("P2_KickHeavy", IE_Pressed, this, &ACryptoFighter::OnKickHeavy);
		PlayerInputComponent->BindAction("P2_Special1", IE_Pressed, this, &ACryptoFighter::OnSpecial1);
		PlayerInputComponent->BindAction("P2_Special2", IE_Pressed, this, &ACryptoFighter::OnSpecial2);
		PlayerInputComponent->BindAction("P2_Block", IE_Pressed, this, &ACryptoFighter::OnBlockPressed);
		PlayerInputComponent->BindAction("P2_Block", IE_Released, this, &ACryptoFighter::OnBlockReleased);
	}
}

void ACryptoFighter::ApplyFighterDefinition(const FFighterDefinition& Definition)
{
	FighterData = Definition;
	CurrentHealth = Definition.MaxHealth;
	CurrentMoonMeter = 0.f;
	GetCharacterMovement()->MaxWalkSpeed = Definition.WalkSpeed;
	SetFighterState(EFighterState::Idle);

	if (FighterVisuals)
	{
		FighterVisuals->BuildBody(Definition.FighterId, Definition.AccentColor);
	}
}

void ACryptoFighter::SetFacingRight(bool bRight)
{
	bFacingRight = bRight;
	const FRotator Yaw(0.f, bFacingRight ? 0.f : 180.f, 0.f);
	SetActorRotation(Yaw);
}

void ACryptoFighter::FaceOpponent(AActor* Opponent)
{
	if (!Opponent)
	{
		return;
	}
	const float DeltaY = Opponent->GetActorLocation().Y - GetActorLocation().Y;
	SetFacingRight(DeltaY > 0.f);
}

float ACryptoFighter::GetHealthPercent() const
{
	const float MaxH = FMath::Max(1.f, FighterData.MaxHealth);
	return CurrentHealth / MaxH;
}

float ACryptoFighter::GetMoonPercent() const
{
	const float MaxM = FMath::Max(1.f, FighterData.MaxMoonMeter);
	return CurrentMoonMeter / MaxM;
}

void ACryptoFighter::AddMoonMeter(float Amount)
{
	CurrentMoonMeter = FMath::Clamp(CurrentMoonMeter + Amount, 0.f, FighterData.MaxMoonMeter);
}

void ACryptoFighter::SetBlocking(bool bNewBlocking)
{
	if (FighterState == EFighterState::KO || FighterState == EFighterState::Hitstun || FighterState == EFighterState::Attack)
	{
		return;
	}
	bIsBlocking = bNewBlocking;
	SetFighterState(bNewBlocking ? EFighterState::Block : EFighterState::Idle);
}

void ACryptoFighter::TryAttack(EAttackSlot Slot)
{
	if (FighterState == EFighterState::KO || FighterState == EFighterState::Hitstun || FighterState == EFighterState::Attack || FighterState == EFighterState::Special)
	{
		return;
	}

	const FMoveDefinition* Move = FindMove(Slot);
	if (!Move)
	{
		return;
	}

	if (Move->MoonCost > 0.f && CurrentMoonMeter < Move->MoonCost)
	{
		UE_LOG(LogCryptoKombat, Verbose, TEXT("%s: not enough Moon Meter for %s"), *GetName(), *Move->MoveId.ToString());
		return;
	}

	CurrentMoonMeter -= Move->MoonCost;
	HitActorsThisAttack.Reset();
	const bool bSpecial = (Slot == EAttackSlot::Special1 || Slot == EAttackSlot::Special2 || Slot == EAttackSlot::Fatality);
	SetFighterState(bSpecial ? EFighterState::Special : EFighterState::Attack);
	StateTimer = Move->ActiveDuration;
	PerformAttackTrace(*Move);
}

void ACryptoFighter::ReceiveHit(float Damage, float HitstunSeconds, ACryptoFighter* Attacker)
{
	if (FighterState == EFighterState::KO)
	{
		return;
	}

	float FinalDamage = Damage;
	if (bIsBlocking && FighterState == EFighterState::Block)
	{
		FinalDamage *= 0.25f; // chip
		AddMoonMeter(5.f);
	}
	else
	{
		SetFighterState(EFighterState::Hitstun);
		PendingHitstun = HitstunSeconds;
		StateTimer = HitstunSeconds;
		bIsBlocking = false;
	}

	CurrentHealth = FMath::Max(0.f, CurrentHealth - FinalDamage);
	AddMoonMeter(FinalDamage * 0.05f);

	if (CurrentHealth <= 0.f)
	{
		SetFighterState(EFighterState::KO);
		StateTimer = 0.f;
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			DisableInput(PC);
		}
		UE_LOG(LogCryptoKombat, Log, TEXT("%s is KO'd"), *FighterData.DisplayName.ToString());
	}
}

void ACryptoFighter::PerformAttackTrace(const FMoveDefinition& Move)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const float Dir = bFacingRight ? 1.f : -1.f;
	const FVector Origin = GetActorLocation() + FVector(0.f, Dir * Move.TraceForwardOffset, 0.f);
	const FVector Half = Move.TraceHalfExtent;
	const FQuat Rot = FQuat::Identity;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(CryptoAttack), false, this);
	TArray<FHitResult> Hits;
	const bool bHit = World->SweepMultiByChannel(
		Hits,
		Origin,
		Origin + FVector(0.f, Dir * 10.f, 0.f),
		Rot,
		ECC_Pawn,
		FCollisionShape::MakeBox(Half),
		Params);

#if !UE_BUILD_SHIPPING
	DrawDebugBox(World, Origin, Half, FColor::Orange, false, 0.2f, 0, 2.f);
#endif

	if (!bHit)
	{
		return;
	}

	for (const FHitResult& Hit : Hits)
	{
		AActor* Other = Hit.GetActor();
		if (!Other || Other == this)
		{
			continue;
		}
		if (HitActorsThisAttack.Contains(Other))
		{
			continue;
		}
		HitActorsThisAttack.Add(Other);

		if (ACryptoFighter* Victim = Cast<ACryptoFighter>(Other))
		{
			Victim->ReceiveHit(Move.Damage, Move.HitstunDuration, this);
			AddMoonMeter(8.f);
			const FVector SparkLoc = Hit.ImpactPoint.IsNearlyZero()
				? (Victim->GetActorLocation() + FVector(0.f, 0.f, 40.f))
				: Hit.ImpactPoint;
			SpawnHitSparkAt(SparkLoc, FighterData.AccentColor);
		}
	}
}

const FMoveDefinition* ACryptoFighter::FindMove(EAttackSlot Slot) const
{
	for (const FMoveDefinition& M : FighterData.Normals)
	{
		if (M.Slot == Slot)
		{
			return &M;
		}
	}
	for (const FMoveDefinition& M : FighterData.Specials)
	{
		if (M.Slot == Slot)
		{
			return &M;
		}
	}
	return nullptr;
}

void ACryptoFighter::SetFighterState(EFighterState NewState)
{
	FighterState = NewState;
	if (FighterVisuals)
	{
		const bool bAttacking = (NewState == EFighterState::Attack || NewState == EFighterState::Special);
		FighterVisuals->SetAttackPose(bAttacking);
	}
}

void ACryptoFighter::TickStateTimers(float DeltaTime)
{
	if (StateTimer > 0.f)
	{
		StateTimer -= DeltaTime;
		if (StateTimer <= 0.f)
		{
			if (FighterState == EFighterState::Attack || FighterState == EFighterState::Special || FighterState == EFighterState::Hitstun)
			{
				SetFighterState(bIsBlocking ? EFighterState::Block : EFighterState::Idle);
			}
		}
	}

	if (FighterState == EFighterState::Walk || FighterState == EFighterState::Idle)
	{
		const FVector Vel = GetVelocity();
		if (FMath::Abs(Vel.Y) > 10.f)
		{
			SetFighterState(EFighterState::Walk);
		}
		else if (FighterState == EFighterState::Walk)
		{
			SetFighterState(EFighterState::Idle);
		}
	}

	if (GetCharacterMovement() && GetCharacterMovement()->IsFalling() && FighterState != EFighterState::KO && FighterState != EFighterState::Hitstun)
	{
		if (FighterState != EFighterState::Attack && FighterState != EFighterState::Special)
		{
			SetFighterState(EFighterState::Jump);
		}
	}
}

void ACryptoFighter::MoveHorizontal(float AxisValue)
{
	if (FighterState == EFighterState::KO || FighterState == EFighterState::Hitstun || FighterState == EFighterState::Attack || FighterState == EFighterState::Special || bIsBlocking)
	{
		return;
	}
	if (FMath::IsNearlyZero(AxisValue))
	{
		return;
	}
	AddMovementInput(FVector(0.f, 1.f, 0.f), AxisValue);
}

void ACryptoFighter::OnJumpPressed()
{
	if (FighterState == EFighterState::KO || FighterState == EFighterState::Hitstun || bIsBlocking)
	{
		return;
	}
	Jump();
	SetFighterState(EFighterState::Jump);
}

void ACryptoFighter::OnPunchLight() { TryAttack(EAttackSlot::LightPunch); }
void ACryptoFighter::OnPunchHeavy() { TryAttack(EAttackSlot::HeavyPunch); }
void ACryptoFighter::OnKickLight() { TryAttack(EAttackSlot::LightKick); }
void ACryptoFighter::OnKickHeavy() { TryAttack(EAttackSlot::HeavyKick); }
void ACryptoFighter::OnSpecial1() { TryAttack(EAttackSlot::Special1); }
void ACryptoFighter::OnSpecial2() { TryAttack(EAttackSlot::Special2); }
void ACryptoFighter::OnBlockPressed() { SetBlocking(true); }
void ACryptoFighter::OnBlockReleased() { SetBlocking(false); }

void ACryptoFighter::HandleMove(const FInputActionValue& Value)
{
	const float Axis = Value.Get<float>();
	MoveHorizontal(Axis);
}

void ACryptoFighter::HandleJump(const FInputActionValue& Value)
{
	OnJumpPressed();
}

void ACryptoFighter::HandleBlock(const FInputActionValue& Value)
{
	const bool bPressed = Value.Get<bool>();
	SetBlocking(bPressed);
}

void ACryptoFighter::SpawnHitSparkAt(const FVector& Location, const FLinearColor& Color)
{
	// Bright flash blended toward accent (yellow-white pop).
	const FLinearColor Flash = FLinearColor(
		FMath::Clamp(Color.R * 0.55f + 0.45f, 0.f, 1.f),
		FMath::Clamp(Color.G * 0.55f + 0.45f, 0.f, 1.f),
		FMath::Clamp(Color.B * 0.35f + 0.2f, 0.f, 1.f),
		1.f);
	AHitSpark::SpawnHitSpark(this, Location, Flash);
}
