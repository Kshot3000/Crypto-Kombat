// Copyright CryptoKombat. Phase 1 scaffold.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "FightCamera.generated.h"

class ACryptoFighter;

/**
 * Locked side-view camera that frames both fighters along the stage Y axis.
 */
UCLASS(Blueprintable)
class CRYPTOKOMBAT_API AFightCamera : public ACameraActor
{
	GENERATED_BODY()

public:
	AFightCamera();

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "CryptoKombat|Camera")
	void SetTrackedFighters(ACryptoFighter* InP1, ACryptoFighter* InP2);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Camera")
	float CameraDistanceX = -900.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Camera")
	float HeightOffsetZ = 80.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Camera")
	float MinOrthoWidth = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Camera")
	float MaxOrthoWidth = 1400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Camera")
	float Padding = 280.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Camera")
	float InterpSpeed = 6.f;

	/** If true, use orthographic framing (classic 2.5D fighter feel). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Camera")
	bool bUseOrtho = true;

protected:
	UPROPERTY()
	TObjectPtr<ACryptoFighter> P1;

	UPROPERTY()
	TObjectPtr<ACryptoFighter> P2;
};
