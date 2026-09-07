// Copyright CryptoKombat. Phase 1 scaffold.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Data/FighterTypes.h"
#include "CryptoFighter.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * Side-view 1v1 fighter pawn.
 * Health, block, stun, facing, Moon Meter, and box-trace attacks.
 */
UCLASS(Blueprintable)
class CRYPTOKOMBAT_API ACryptoFighter : public ACharacter
{
	GENERATED_BODY()

public:
	ACryptoFighter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/** Apply roster definition (health caps, moves, tint). */
	UFUNCTION(BlueprintCallable, Category = "CryptoKombat|Fighter")
	void ApplyFighterDefinition(const FFighterDefinition& Definition);

	UFUNCTION(BlueprintCallable, Category = "CryptoKombat|Fighter")
	void SetFacingRight(bool bRight);

	UFUNCTION(BlueprintPure, Category = "CryptoKombat|Fighter")
	bool IsFacingRight() const { return bFacingRight; }

	UFUNCTION(BlueprintCallable, Category = "CryptoKombat|Combat")
	void TryAttack(EAttackSlot Slot);

	UFUNCTION(BlueprintCallable, Category = "CryptoKombat|Combat")
	void SetBlocking(bool bNewBlocking);

	UFUNCTION(BlueprintCallable, Category = "CryptoKombat|Combat")
	void ReceiveHit(float Damage, float HitstunSeconds, ACryptoFighter* Attacker);

	UFUNCTION(BlueprintPure, Category = "CryptoKombat|Combat")
	EFighterState GetFighterState() const { return FighterState; }

	UFUNCTION(BlueprintPure, Category = "CryptoKombat|Combat")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintPure, Category = "CryptoKombat|Combat")
	float GetMoonPercent() const;

	UFUNCTION(BlueprintCallable, Category = "CryptoKombat|Combat")
	void AddMoonMeter(float Amount);

	UFUNCTION(BlueprintCallable, Category = "CryptoKombat|Combat")
	void FaceOpponent(AActor* Opponent);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Fighter")
	FFighterDefinition FighterData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CryptoKombat|Combat")
	float CurrentHealth = 1000.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CryptoKombat|Combat")
	float CurrentMoonMeter = 0.f;

	/** Player index 0 = P1, 1 = P2 (same-keyboard versus). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Input")
	int32 PlayerIndex = 0;

protected:
	void MoveHorizontal(float AxisValue);
	void OnJumpPressed();
	void OnPunchLight();
	void OnPunchHeavy();
	void OnKickLight();
	void OnKickHeavy();
	void OnSpecial1();
	void OnSpecial2();
	void OnBlockPressed();
	void OnBlockReleased();

	void SetFighterState(EFighterState NewState);
	const FMoveDefinition* FindMove(EAttackSlot Slot) const;
	void PerformAttackTrace(const FMoveDefinition& Move);
	void TickStateTimers(float DeltaTime);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CryptoKombat|Combat")
	EFighterState FighterState = EFighterState::Idle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CryptoKombat|Combat")
	bool bFacingRight = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CryptoKombat|Combat")
	bool bIsBlocking = false;

	float StateTimer = 0.f;
	float PendingHitstun = 0.f;

	/** Actors already hit by the current attack (prevents multi-hit per swing). */
	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> HitActorsThisAttack;

	/** Enhanced Input assets — assign in BP / DefaultInput docs. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CryptoKombat|Input")
	TObjectPtr<UInputMappingContext> MappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CryptoKombat|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CryptoKombat|Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CryptoKombat|Input")
	TObjectPtr<UInputAction> PunchLightAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CryptoKombat|Input")
	TObjectPtr<UInputAction> PunchHeavyAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CryptoKombat|Input")
	TObjectPtr<UInputAction> KickLightAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CryptoKombat|Input")
	TObjectPtr<UInputAction> KickHeavyAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CryptoKombat|Input")
	TObjectPtr<UInputAction> Special1Action;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CryptoKombat|Input")
	TObjectPtr<UInputAction> Special2Action;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CryptoKombat|Input")
	TObjectPtr<UInputAction> BlockAction;

	void HandleMove(const FInputActionValue& Value);
	void HandleJump(const FInputActionValue& Value);
	void HandleBlock(const FInputActionValue& Value);
};
