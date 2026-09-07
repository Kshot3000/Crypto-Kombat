// Copyright CryptoKombat. Phase 1 scaffold.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FightGameState.generated.h"

UENUM(BlueprintType)
enum class ERoundPhase : uint8
{
	PreRound		UMETA(DisplayName = "Pre Round"),
	Fighting		UMETA(DisplayName = "Fighting"),
	RoundEnd		UMETA(DisplayName = "Round End"),
	MatchEnd		UMETA(DisplayName = "Match End"),
	FinishHim		UMETA(DisplayName = "Finish Window")
};

/**
 * Replicated-friendly match state for best-of-3 rounds + timer.
 * Phase 1 is local-only but keeps GameState for HUD binding.
 */
UCLASS(Blueprintable)
class CRYPTOKOMBAT_API AFightGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AFightGameState();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CryptoKombat|Match")
	ERoundPhase RoundPhase = ERoundPhase::PreRound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CryptoKombat|Match")
	int32 CurrentRound = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Match")
	int32 RoundsToWin = 2; // best of 3

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CryptoKombat|Match")
	int32 P1RoundsWon = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CryptoKombat|Match")
	int32 P2RoundsWon = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Match")
	float RoundDurationSeconds = 99.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CryptoKombat|Match")
	float RoundTimeRemaining = 99.f;

	UFUNCTION(BlueprintCallable, Category = "CryptoKombat|Match")
	void ResetRoundClock();

	UFUNCTION(BlueprintCallable, Category = "CryptoKombat|Match")
	void TickRoundClock(float DeltaSeconds);

	UFUNCTION(BlueprintPure, Category = "CryptoKombat|Match")
	bool IsMatchOver() const;
};
