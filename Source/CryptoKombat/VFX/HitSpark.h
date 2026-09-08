// Copyright CryptoKombat. Phase 1 scaffold.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitSpark.generated.h"

class UStaticMeshComponent;
class UStaticMesh;
class UMaterialInterface;

/**
 * Short-lived impact burst: colored sphere that scales up then destroys (~0.25s).
 * Spawned from attack traces — no Niagara / Content assets required.
 */
UCLASS()
class CRYPTOKOMBAT_API AHitSpark : public AActor
{
	GENERATED_BODY()

public:
	AHitSpark();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/** Tint + lifetime setup before or right after spawn. */
	UFUNCTION(BlueprintCallable, Category = "CryptoKombat|VFX")
	void Configure(const FLinearColor& Color, float InLifetime = 0.25f);

	/** Convenience: spawn at world location with accent color. */
	UFUNCTION(BlueprintCallable, Category = "CryptoKombat|VFX", meta = (WorldContext = "WorldContextObject"))
	static AHitSpark* SpawnHitSpark(UObject* WorldContextObject, const FVector& Location, const FLinearColor& Color);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|VFX")
	float Lifetime = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|VFX")
	float StartScale = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|VFX")
	float EndScale = 0.85f;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CryptoKombat|VFX")
	TObjectPtr<UStaticMeshComponent> SparkMesh;

	UPROPERTY()
	TObjectPtr<UStaticMesh> SphereMesh;

	UPROPERTY()
	TObjectPtr<UMaterialInterface> BaseMaterial;

	float Age = 0.f;
	FLinearColor SparkColor = FLinearColor(1.f, 0.95f, 0.4f, 1.f);

	void ApplyColor(const FLinearColor& Color);
};
