// Copyright CryptoKombat. Phase 1 scaffold.

#include "Characters/FighterVisuals.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

UFighterVisuals::UFighterVisuals()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	SetIsReplicatedByDefault(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeFinder.Succeeded())
	{
		CubeMesh = CubeFinder.Object;
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereFinder(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereFinder.Succeeded())
	{
		SphereMesh = SphereFinder.Object;
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylFinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (CylFinder.Succeeded())
	{
		CylinderMesh = CylFinder.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatFinder(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (MatFinder.Succeeded())
	{
		BaseMaterial = MatFinder.Object;
	}
}

void UFighterVisuals::BeginPlay()
{
	Super::BeginPlay();
	if (!bBuilt)
	{
		BuildBody(NAME_None, CurrentAccent);
	}
}

void UFighterVisuals::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateAttackPose(DeltaTime);
}

void UFighterVisuals::ResolveProportions(FName FighterId)
{
	// Default balanced silhouette.
	BodyProportion = FVector(1.f, 1.f, 1.f);

	if (FighterId == TEXT("SolFlash"))
	{
		// Lean / tall sprinter.
		BodyProportion = FVector(0.85f, 0.9f, 1.08f);
	}
	else if (FighterId == TEXT("CharlesEpoch"))
	{
		// Broader torso / formal presence.
		BodyProportion = FVector(1.15f, 1.12f, 0.98f);
	}
	else if (FighterId == TEXT("VitalSpark"))
	{
		// Lanky hoodie-coder.
		BodyProportion = FVector(0.9f, 0.95f, 1.05f);
	}
	else if (FighterId == TEXT("CZChain"))
	{
		BodyProportion = FVector(1.2f, 1.15f, 1.0f);
	}
	else if (FighterId == TEXT("SatoshiShadow"))
	{
		BodyProportion = FVector(1.0f, 1.0f, 1.02f);
	}
}

UStaticMeshComponent* UFighterVisuals::AddPart(
	UStaticMesh* Mesh,
	FName Name,
	USceneComponent* Parent,
	const FVector& RelativeLocation,
	const FVector& RelativeScale,
	const FRotator& RelativeRotation)
{
	if (!Mesh || !Parent)
	{
		return nullptr;
	}

	UStaticMeshComponent* Comp = NewObject<UStaticMeshComponent>(GetOwner(), Name);
	if (!Comp)
	{
		return nullptr;
	}

	Comp->SetupAttachment(Parent);
	Comp->SetStaticMesh(Mesh);
	Comp->SetRelativeLocation(RelativeLocation);
	Comp->SetRelativeScale3D(RelativeScale);
	Comp->SetRelativeRotation(RelativeRotation);
	Comp->SetMobility(EComponentMobility::Movable);
	Comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Comp->SetCastShadow(true);
	Comp->RegisterComponent();
	BodyParts.Add(Comp);
	return Comp;
}

void UFighterVisuals::ApplyColorToPart(UStaticMeshComponent* MeshComp, const FLinearColor& Color)
{
	if (!MeshComp || !BaseMaterial)
	{
		return;
	}

	UMaterialInstanceDynamic* DynMat = MeshComp->CreateDynamicMaterialInstance(0, BaseMaterial);
	if (DynMat)
	{
		DynMat->SetVectorParameterValue(TEXT("Color"), Color);
		DynMat->SetVectorParameterValue(TEXT("BaseColor"), Color);
	}
}

void UFighterVisuals::BuildBody(FName FighterId, const FLinearColor& AccentColor)
{
	// Tear down previous parts if rebuilding.
	for (UStaticMeshComponent* Part : BodyParts)
	{
		if (Part)
		{
			Part->DestroyComponent();
		}
	}
	BodyParts.Reset();
	if (BodyRoot)
	{
		BodyRoot->DestroyComponent();
		BodyRoot = nullptr;
	}

	ResolveProportions(FighterId);
	CurrentAccent = AccentColor;

	BodyRoot = NewObject<USceneComponent>(GetOwner(), TEXT("FighterBodyRoot"));
	BodyRoot->SetupAttachment(this);
	BodyRoot->SetRelativeLocation(FVector(0.f, 0.f, -88.f)); // capsule half-height ~88uu
	BodyRoot->SetRelativeScale3D(BodyProportion);
	BodyRoot->RegisterComponent();

	// Side-view silhouette: depth on X thin, width on Y, height on Z.
	// Engine BasicShapes are 100uu; scales are in "hundreds of uu".

	// Torso — main body block
	Torso = AddPart(CubeMesh, TEXT("Torso"), BodyRoot,
		FVector(0.f, 0.f, 70.f),
		FVector(0.35f, 0.45f, 0.55f));

	// Head — sphere on top
	Head = AddPart(SphereMesh, TEXT("Head"), BodyRoot,
		FVector(0.f, 0.f, 115.f),
		FVector(0.32f, 0.32f, 0.32f));

	// Arms — cylinders rotated to hang at sides (Y = along stage forward for punch pose)
	// Idle: arms at sides pointing slightly forward on Y for readable profile.
	ArmL = AddPart(CylinderMesh, TEXT("ArmL"), BodyRoot,
		FVector(0.f, -28.f, 78.f),
		FVector(0.12f, 0.12f, 0.35f),
		FRotator(0.f, 0.f, 90.f));

	ArmR = AddPart(CylinderMesh, TEXT("ArmR"), BodyRoot,
		FVector(0.f, 28.f, 78.f),
		FVector(0.12f, 0.12f, 0.35f),
		FRotator(0.f, 0.f, -90.f));

	// Legs
	LegL = AddPart(CubeMesh, TEXT("LegL"), BodyRoot,
		FVector(0.f, -12.f, 22.f),
		FVector(0.22f, 0.18f, 0.4f));

	LegR = AddPart(CubeMesh, TEXT("LegR"), BodyRoot,
		FVector(0.f, 12.f, 22.f),
		FVector(0.22f, 0.18f, 0.4f));

	ApplyAccentColor(AccentColor);
	bBuilt = true;
	bAttackPose = false;
	AttackPoseAlpha = 0.f;
}

void UFighterVisuals::ApplyAccentColor(const FLinearColor& AccentColor)
{
	CurrentAccent = AccentColor;

	// Slightly darker torso / brighter extremities for readable contrast.
	const FLinearColor TorsoColor = AccentColor * 0.75f;
	const FLinearColor HeadColor = FLinearColor(
		FMath::Min(1.f, AccentColor.R * 1.15f),
		FMath::Min(1.f, AccentColor.G * 1.15f),
		FMath::Min(1.f, AccentColor.B * 1.15f),
		1.f);
	const FLinearColor LimbColor = AccentColor;

	ApplyColorToPart(Torso, TorsoColor);
	ApplyColorToPart(Head, HeadColor);
	ApplyColorToPart(ArmL, LimbColor);
	ApplyColorToPart(ArmR, LimbColor);
	ApplyColorToPart(LegL, LimbColor * 0.85f);
	ApplyColorToPart(LegR, LimbColor * 0.85f);
}

void UFighterVisuals::SetAttackPose(bool bAttacking)
{
	bAttackPose = bAttacking;
}

void UFighterVisuals::UpdateAttackPose(float DeltaTime)
{
	const float Target = bAttackPose ? 1.f : 0.f;
	AttackPoseAlpha = FMath::FInterpTo(AttackPoseAlpha, Target, DeltaTime, 14.f);

	if (!ArmR || !ArmL || !BodyRoot)
	{
		return;
	}

	// Thrust the "lead" arm forward along local Y (stage axis) and squash torso slightly.
	const float Punch = AttackPoseAlpha;
	ArmR->SetRelativeLocation(FVector(0.f, 28.f + Punch * 45.f, 78.f + Punch * 5.f));
	ArmR->SetRelativeScale3D(FVector(0.12f, 0.12f + Punch * 0.08f, 0.35f + Punch * 0.15f));
	ArmL->SetRelativeLocation(FVector(0.f, -28.f - Punch * 10.f, 78.f));

	if (Torso)
	{
		Torso->SetRelativeScale3D(FVector(0.35f, 0.45f + Punch * 0.08f, 0.55f - Punch * 0.04f));
	}
}
