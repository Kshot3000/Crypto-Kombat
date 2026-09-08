// Copyright CryptoKombat. Phase 1 scaffold.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "FighterVisuals.generated.h"

class UStaticMeshComponent;
class UStaticMesh;
class UMaterialInterface;

/**
 * Procedural stylized block-fighter body (BasicShapes hierarchy).
 * Readable side-view silhouette; tinted via AccentColor MIDs.
 * Collision stays on the Character capsule — this is visual-only.
 */
UCLASS(ClassGroup = (CryptoKombat), meta = (BlueprintSpawnableComponent))
class CRYPTOKOMBAT_API UFighterVisuals : public USceneComponent
{
	GENERATED_BODY()

public:
	UFighterVisuals();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Build / rebuild body parts and apply accent + proportion variance. */
	UFUNCTION(BlueprintCallable, Category = "CryptoKombat|Visuals")
	void BuildBody(FName FighterId, const FLinearColor& AccentColor);

	/** Re-tint all parts (e.g. after ApplyFighterDefinition). */
	UFUNCTION(BlueprintCallable, Category = "CryptoKombat|Visuals")
	void ApplyAccentColor(const FLinearColor& AccentColor);

	/** Drive brief attack squash (arms thrust forward). */
	UFUNCTION(BlueprintCallable, Category = "CryptoKombat|Visuals")
	void SetAttackPose(bool bAttacking);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CryptoKombat|Visuals")
	FLinearColor CurrentAccent = FLinearColor::White;

protected:
	UPROPERTY()
	TObjectPtr<UStaticMesh> CubeMesh;

	UPROPERTY()
	TObjectPtr<UStaticMesh> SphereMesh;

	UPROPERTY()
	TObjectPtr<UStaticMesh> CylinderMesh;

	UPROPERTY()
	TObjectPtr<UMaterialInterface> BaseMaterial;

	UPROPERTY()
	TObjectPtr<USceneComponent> BodyRoot;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> Torso;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> Head;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> ArmL;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> ArmR;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> LegL;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> LegR;

	UPROPERTY()
	TArray<TObjectPtr<UStaticMeshComponent>> BodyParts;

	bool bBuilt = false;
	bool bAttackPose = false;
	float AttackPoseAlpha = 0.f;

	/** Global proportion scale (leaner / broader by FighterId). */
	FVector BodyProportion = FVector(1.f, 1.f, 1.f);

	UStaticMeshComponent* AddPart(
		UStaticMesh* Mesh,
		FName Name,
		USceneComponent* Parent,
		const FVector& RelativeLocation,
		const FVector& RelativeScale,
		const FRotator& RelativeRotation = FRotator::ZeroRotator);

	void ApplyColorToPart(UStaticMeshComponent* MeshComp, const FLinearColor& Color);
	void ResolveProportions(FName FighterId);
	void UpdateAttackPose(float DeltaTime);
};
