// Copyright CryptoKombat. Phase 1 scaffold.

#include "Arena/BlockchainColosseum.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"
#include "CryptoKombat.h"

ABlockchainColosseum::ABlockchainColosseum()
{
	PrimaryActorTick.bCanEverTick = false;

	ArenaRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ArenaRoot"));
	SetRootComponent(ArenaRoot);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeFinder.Succeeded())
	{
		CubeMesh = CubeFinder.Object;
	}

	// Unlit / basic material — Engine BasicShape material accepts BaseColor where available.
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatFinder(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (MatFinder.Succeeded())
	{
		BaseMaterial = MatFinder.Object;
	}
}

void ABlockchainColosseum::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	BuildArena();
}

void ABlockchainColosseum::BeginPlay()
{
	Super::BeginPlay();
	if (!bBuilt)
	{
		BuildArena();
	}
}

FVector ABlockchainColosseum::GetP1Spawn() const
{
	const FVector Origin = GetActorLocation();
	return Origin + FVector(0.f, -SpawnHalfSeparation, FighterSpawnZ);
}

FVector ABlockchainColosseum::GetP2Spawn() const
{
	const FVector Origin = GetActorLocation();
	return Origin + FVector(0.f, SpawnHalfSeparation, FighterSpawnZ);
}

void ABlockchainColosseum::ClearBuiltMeshes()
{
	for (UStaticMeshComponent* Comp : BuiltMeshes)
	{
		if (Comp)
		{
			Comp->DestroyComponent();
		}
	}
	BuiltMeshes.Reset();
	bBuilt = false;
}

void ABlockchainColosseum::ApplyColor(UStaticMeshComponent* MeshComp, const FLinearColor& Color)
{
	if (!MeshComp || !BaseMaterial)
	{
		return;
	}

	UMaterialInstanceDynamic* DynMat = MeshComp->CreateDynamicMaterialInstance(0, BaseMaterial);
	if (DynMat)
	{
		// BasicShapeMaterial exposes Color; also try common param names for robustness.
		DynMat->SetVectorParameterValue(TEXT("Color"), Color);
		DynMat->SetVectorParameterValue(TEXT("BaseColor"), Color);
	}
}

UStaticMeshComponent* ABlockchainColosseum::AddBox(
	const FName& Name,
	const FVector& RelativeLocation,
	const FVector& Scale3D,
	const FLinearColor& Color,
	bool bEnableCollision)
{
	if (!CubeMesh)
	{
		return nullptr;
	}

	UStaticMeshComponent* MeshComp = NewObject<UStaticMeshComponent>(this, Name);
	if (!MeshComp)
	{
		return nullptr;
	}

	MeshComp->SetupAttachment(ArenaRoot);
	MeshComp->SetStaticMesh(CubeMesh);
	MeshComp->SetRelativeLocation(RelativeLocation);
	MeshComp->SetRelativeScale3D(Scale3D);
	MeshComp->SetMobility(EComponentMobility::Movable);
	MeshComp->SetCollisionEnabled(bEnableCollision ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	if (bEnableCollision)
	{
		MeshComp->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	}
	MeshComp->RegisterComponent();
	ApplyColor(MeshComp, Color);
	BuiltMeshes.Add(MeshComp);
	return MeshComp;
}

void ABlockchainColosseum::BuildArena()
{
	ClearBuiltMeshes();

	if (!CubeMesh)
	{
		UE_LOG(LogCryptoKombat, Warning, TEXT("BlockchainColosseum: engine Cube mesh missing — arena not built."));
		return;
	}

	// Engine BasicShapes/Cube is 100uu per side; Scale3D is therefore in "hundreds of uu".
	const float HalfY = ArenaHalfWidth;
	const float FloorHalfX = FloorDepthX * 0.5f;
	const float FloorZ = -FloorThickness * 0.5f;

	// --- Floor slab (wide on Y, thin on Z) ---
	AddBox(
		TEXT("Floor"),
		FVector(0.f, 0.f, FloorZ),
		FVector(FloorDepthX / 100.f, (HalfY * 2.f + 80.f) / 100.f, FloorThickness / 100.f),
		FloorColor,
		true);

	// Bright center strip (logo / decal stand-in)
	AddBox(
		TEXT("CenterStrip"),
		FVector(0.f, 0.f, 2.f),
		FVector((FloorDepthX * 0.55f) / 100.f, 0.35f, 0.04f),
		CenterStripColor,
		false);

	// --- Soft side walls (keep fighters in bounds) ---
	const float WallThickness = 40.f;
	const float WallCenterZ = WallHeight * 0.5f;
	AddBox(
		TEXT("WallLeft"),
		FVector(0.f, -HalfY - WallThickness * 0.5f, WallCenterZ),
		FVector(FloorDepthX / 100.f, WallThickness / 100.f, WallHeight / 100.f),
		WallColor,
		true);
	AddBox(
		TEXT("WallRight"),
		FVector(0.f, HalfY + WallThickness * 0.5f, WallCenterZ),
		FVector(FloorDepthX / 100.f, WallThickness / 100.f, WallHeight / 100.f),
		WallColor,
		true);

	// Neon trim on walls
	AddBox(
		TEXT("WallTrimLeft"),
		FVector(0.f, -HalfY - WallThickness * 0.5f, WallHeight + 8.f),
		FVector((FloorDepthX * 0.9f) / 100.f, 0.15f, 0.12f),
		AccentCyan,
		false);
	AddBox(
		TEXT("WallTrimRight"),
		FVector(0.f, HalfY + WallThickness * 0.5f, WallHeight + 8.f),
		FVector((FloorDepthX * 0.9f) / 100.f, 0.15f, 0.12f),
		AccentMagenta,
		false);

	// --- Back wall / backdrop panels ---
	const float BackdropX = FloorHalfX + 30.f;
	AddBox(
		TEXT("BackdropMain"),
		FVector(BackdropX, 0.f, WallHeight * 0.55f),
		FVector(0.2f, (HalfY * 2.f) / 100.f, (WallHeight * 1.1f) / 100.f),
		BackdropColor,
		false);

	// Accent backdrop strips
	AddBox(
		TEXT("BackdropCyan"),
		FVector(BackdropX - 5.f, -HalfY * 0.45f, WallHeight * 0.7f),
		FVector(0.12f, 1.8f, 2.4f),
		AccentCyan,
		false);
	AddBox(
		TEXT("BackdropMagenta"),
		FVector(BackdropX - 5.f, HalfY * 0.45f, WallHeight * 0.7f),
		FVector(0.12f, 1.8f, 2.4f),
		AccentMagenta,
		false);

	// --- Decorative pillars / "block" stacks ---
	const float PillarYOffsets[] = { -HalfY * 0.7f, -HalfY * 0.35f, HalfY * 0.35f, HalfY * 0.7f };
	for (int32 i = 0; i < 4; ++i)
	{
		const float PY = PillarYOffsets[i];
		const bool bCyan = (i % 2) == 0;
		const FLinearColor PillarAccent = bCyan ? AccentCyan : AccentMagenta;
		const FName BaseName(*FString::Printf(TEXT("Pillar_%d"), i));

		// Stack of "blocks"
		for (int32 Stack = 0; Stack < 3; ++Stack)
		{
			const float SZ = 30.f + Stack * 55.f;
			const float Scl = 0.55f - Stack * 0.05f;
			AddBox(
				*FString::Printf(TEXT("Pillar_%d_Block_%d"), i, Stack),
				FVector(-FloorHalfX + 40.f, PY, SZ),
				FVector(Scl, Scl, 0.5f),
				Stack == 2 ? PillarAccent : WallColor,
				false);
		}
	}

	// --- Raised mempool pit rim (center ring stand-in: four edge pieces) ---
	const float RimHalf = 120.f;
	const float RimZ = 12.f;
	const FLinearColor RimColor = FLinearColor(
		FMath::Lerp(AccentCyan.R, AccentMagenta.R, 0.5f),
		FMath::Lerp(AccentCyan.G, AccentMagenta.G, 0.5f),
		FMath::Lerp(AccentCyan.B, AccentMagenta.B, 0.5f),
		1.f);

	AddBox(TEXT("MempoolRim_N"), FVector(RimHalf, 0.f, RimZ), FVector(0.15f, 2.4f, 0.18f), RimColor, true);
	AddBox(TEXT("MempoolRim_S"), FVector(-RimHalf, 0.f, RimZ), FVector(0.15f, 2.4f, 0.18f), RimColor, true);
	AddBox(TEXT("MempoolRim_W"), FVector(0.f, -RimHalf, RimZ), FVector(2.4f, 0.15f, 0.18f), RimColor, true);
	AddBox(TEXT("MempoolRim_E"), FVector(0.f, RimHalf, RimZ), FVector(2.4f, 0.15f, 0.18f), RimColor, true);

	// Slightly sunken pit floor tint inside rim
	AddBox(
		TEXT("MempoolPit"),
		FVector(0.f, 0.f, 1.f),
		FVector(2.0f, 2.0f, 0.03f),
		FLinearColor(0.12f, 0.02f, 0.18f, 1.f),
		false);

	// --- Floating candle-chart panels (thin boxes hovering) ---
	struct FChartSpec
	{
		FVector Loc;
		FVector Scale;
		FLinearColor Color;
	};
	const FChartSpec Charts[] = {
		{ FVector(-80.f, -220.f, 220.f), FVector(0.08f, 1.2f, 0.9f), AccentCyan },
		{ FVector(-60.f, -180.f, 260.f), FVector(0.08f, 0.7f, 1.3f), AccentMagenta },
		{ FVector(-70.f, 200.f, 230.f), FVector(0.08f, 1.0f, 1.1f), AccentCyan },
		{ FVector(-50.f, 260.f, 280.f), FVector(0.08f, 0.6f, 0.8f), AccentMagenta },
		{ FVector(-90.f, 40.f, 300.f), FVector(0.08f, 0.9f, 1.5f), AccentCyan },
	};
	for (int32 i = 0; i < UE_ARRAY_COUNT(Charts); ++i)
	{
		AddBox(
			*FString::Printf(TEXT("CandlePanel_%d"), i),
			Charts[i].Loc,
			Charts[i].Scale,
			Charts[i].Color,
			false);
	}


	// --- Neon floor edge strips (strong contrast vs dark floor) ---
	AddBox(
		TEXT("FloorNeonFront"),
		FVector(FloorHalfX - 10.f, 0.f, 3.f),
		FVector(0.08f, (HalfY * 1.9f) / 100.f, 0.06f),
		AccentCyan,
		false);
	AddBox(
		TEXT("FloorNeonBack"),
		FVector(-FloorHalfX + 10.f, 0.f, 3.f),
		FVector(0.08f, (HalfY * 1.9f) / 100.f, 0.06f),
		AccentMagenta,
		false);

	// Overhead light boxes (emissive stand-ins hanging above the stage)
	const FLinearColor OverheadWarm = FLinearColor(1.f, 0.95f, 0.75f, 1.f);
	AddBox(
		TEXT("OverheadLight_L"),
		FVector(0.f, -HalfY * 0.4f, WallHeight + 40.f),
		FVector(1.2f, 0.8f, 0.2f),
		OverheadWarm,
		false);
	AddBox(
		TEXT("OverheadLight_R"),
		FVector(0.f, HalfY * 0.4f, WallHeight + 40.f),
		FVector(1.2f, 0.8f, 0.2f),
		OverheadWarm,
		false);
	AddBox(
		TEXT("OverheadLight_C"),
		FVector(0.f, 0.f, WallHeight + 55.f),
		FVector(0.9f, 1.4f, 0.18f),
		AccentCyan,
		false);

	// Vertical neon ribs on backdrop
	for (int32 Rib = 0; Rib < 5; ++Rib)
	{
		const float RY = -HalfY * 0.8f + Rib * (HalfY * 1.6f / 4.f);
		const FLinearColor RibColor = (Rib % 2 == 0) ? AccentCyan : AccentMagenta;
		AddBox(
			*FString::Printf(TEXT("BackdropRib_%d"), Rib),
			FVector(BackdropX - 8.f, RY, WallHeight * 0.5f),
			FVector(0.06f, 0.08f, (WallHeight * 0.95f) / 100.f),
			RibColor,
			false);
	}

	// Corner neon pylons
	AddBox(TEXT("Pylon_FL"), FVector(-FloorHalfX + 20.f, -HalfY + 20.f, 80.f), FVector(0.25f, 0.25f, 1.5f), AccentCyan, false);
	AddBox(TEXT("Pylon_FR"), FVector(-FloorHalfX + 20.f, HalfY - 20.f, 80.f), FVector(0.25f, 0.25f, 1.5f), AccentMagenta, false);

	bBuilt = true;
	UE_LOG(LogCryptoKombat, Log, TEXT("BlockchainColosseum built (half-width=%.0f, walls=%.0f)"), ArenaHalfWidth, WallHeight);
}
