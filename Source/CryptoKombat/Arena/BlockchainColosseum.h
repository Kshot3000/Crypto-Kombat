// Copyright CryptoKombat. Phase 1 scaffold.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlockchainColosseum.generated.h"

class UStaticMeshComponent;
class UStaticMesh;
class UMaterialInterface;

/**
 * Procedural starter arena — Blockchain Colosseum.
 * Built entirely from engine BasicShapes so PIE works without .umap assets.
 * Drop into a level or let AFightGameMode auto-spawn it.
 */
UCLASS(Blueprintable)
class CRYPTOKOMBAT_API ABlockchainColosseum : public AActor
{
	GENERATED_BODY()

public:
	ABlockchainColosseum();

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

	/** Rebuild floor, walls, pillars, mempool rim, and chart panels from EditAnywhere props. */
	UFUNCTION(BlueprintCallable, Category = "CryptoKombat|Arena")
	void BuildArena();

	UFUNCTION(BlueprintPure, Category = "CryptoKombat|Arena")
	FVector GetP1Spawn() const;

	UFUNCTION(BlueprintPure, Category = "CryptoKombat|Arena")
	FVector GetP2Spawn() const;

	/** Half-width along stage Y (soft walls near ±ArenaHalfWidth). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Arena|Layout")
	float ArenaHalfWidth = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Arena|Layout")
	float FloorThickness = 40.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Arena|Layout")
	float FloorDepthX = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Arena|Layout")
	float WallHeight = 320.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Arena|Layout")
	float SpawnHalfSeparation = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Arena|Layout")
	float FighterSpawnZ = 100.f;

	/** Neon cyan accent (crypto vibe). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Arena|Colors")
	FLinearColor AccentCyan = FLinearColor(0.05f, 0.95f, 1.f, 1.f);

	/** Neon magenta accent. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Arena|Colors")
	FLinearColor AccentMagenta = FLinearColor(1.f, 0.1f, 0.85f, 1.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Arena|Colors")
	FLinearColor FloorColor = FLinearColor(0.04f, 0.05f, 0.09f, 1.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Arena|Colors")
	FLinearColor WallColor = FLinearColor(0.08f, 0.09f, 0.14f, 1.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Arena|Colors")
	FLinearColor BackdropColor = FLinearColor(0.02f, 0.03f, 0.08f, 1.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Arena|Colors")
	FLinearColor CenterStripColor = FLinearColor(0.15f, 0.95f, 0.9f, 1.f);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CryptoKombat|Arena")
	TObjectPtr<USceneComponent> ArenaRoot;

	UPROPERTY()
	TObjectPtr<UStaticMesh> CubeMesh;

	UPROPERTY()
	TObjectPtr<UMaterialInterface> BaseMaterial;

	UPROPERTY()
	TArray<TObjectPtr<UStaticMeshComponent>> BuiltMeshes;

	bool bBuilt = false;

	UStaticMeshComponent* AddBox(
		const FName& Name,
		const FVector& RelativeLocation,
		const FVector& Scale3D,
		const FLinearColor& Color,
		bool bEnableCollision);

	void ApplyColor(UStaticMeshComponent* MeshComp, const FLinearColor& Color);
	void ClearBuiltMeshes();
};
