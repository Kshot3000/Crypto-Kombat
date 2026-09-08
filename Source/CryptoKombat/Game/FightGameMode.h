// Copyright CryptoKombat. Phase 1 scaffold.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/FighterTypes.h"
#include "FightGameMode.generated.h"

class ACryptoFighter;
class AFightGameState;
class AFightCamera;
class ABlockchainColosseum;
class AFightHUD;

/**
 * Local versus GameMode: best-of-3 rounds, countdown timer, spawn two fighters.
 */
UCLASS(Blueprintable)
class CRYPTOKOMBAT_API AFightGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFightGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "CryptoKombat|Match")
	void StartMatchFlow();

	UFUNCTION(BlueprintCallable, Category = "CryptoKombat|Match")
	void BeginRound();

	UFUNCTION(BlueprintCallable, Category = "CryptoKombat|Match")
	void EndRound(int32 WinningPlayerIndex);

	UFUNCTION(BlueprintCallable, Category = "CryptoKombat|Match")
	void CheckForKO();

	UFUNCTION(BlueprintPure, Category = "CryptoKombat|Match")
	ACryptoFighter* GetP1Fighter() const { return P1Fighter; }

	UFUNCTION(BlueprintPure, Category = "CryptoKombat|Match")
	ACryptoFighter* GetP2Fighter() const { return P2Fighter; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Spawn")
	TSubclassOf<ACryptoFighter> FighterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Spawn")
	TSubclassOf<AFightCamera> CameraClass;

	/** Procedural arena class (Blockchain Colosseum by default). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Arena")
	TSubclassOf<ABlockchainColosseum> ArenaClass;

	/** If true and no ABlockchainColosseum exists in the world, spawn one before fighters. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Arena")
	bool bAutoSpawnArena = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Spawn")
	FVector P1SpawnLocation = FVector(0.f, -300.f, 100.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Spawn")
	FVector P2SpawnLocation = FVector(0.f, 300.f, 100.f);

	/** Featured default matchup: Vital Spark vs Charles Epoch. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Roster")
	FName P1FighterId = TEXT("VitalSpark");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Roster")
	FName P2FighterId = TEXT("CharlesEpoch");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Match")
	float PreRoundDelay = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Match")
	float RoundEndDelay = 2.5f;

protected:
	UPROPERTY()
	TObjectPtr<ACryptoFighter> P1Fighter;

	UPROPERTY()
	TObjectPtr<ACryptoFighter> P2Fighter;

	UPROPERTY()
	TObjectPtr<AFightCamera> FightCamera;

	UPROPERTY()
	TObjectPtr<ABlockchainColosseum> ActiveArena;

	FFighterDefinition FindRosterFighter(FName FighterId) const;
	void EnsureArena();
	void SpawnFighters();
	void SpawnCamera();
	void UpdateFacing();
	AFightGameState* GetFightState() const;

	float PhaseTimer = 0.f;
	bool bMatchStarted = false;
};
