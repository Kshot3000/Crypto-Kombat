// Copyright CryptoKombat. Phase 1 scaffold.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/FighterTypes.h"
#include "FightGameMode.generated.h"

class ACryptoFighter;
class AFightGameState;
class AFightCamera;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Spawn")
	TSubclassOf<ACryptoFighter> FighterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Spawn")
	TSubclassOf<AFightCamera> CameraClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Spawn")
	FVector P1SpawnLocation = FVector(0.f, -300.f, 100.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Spawn")
	FVector P2SpawnLocation = FVector(0.f, 300.f, 100.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Roster")
	FName P1FighterId = TEXT("SatoshiShadow");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Roster")
	FName P2FighterId = TEXT("BrianCoin");

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

	FFighterDefinition FindRosterFighter(FName FighterId) const;
	void SpawnFighters();
	void SpawnCamera();
	void UpdateFacing();
	AFightGameState* GetFightState() const;

	float PhaseTimer = 0.f;
	bool bMatchStarted = false;
};
