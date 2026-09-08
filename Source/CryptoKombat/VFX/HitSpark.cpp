// Copyright CryptoKombat. Phase 1 scaffold.

#include "VFX/HitSpark.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

AHitSpark::AHitSpark()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = false;

	SparkMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SparkMesh"));
	SetRootComponent(SparkMesh);
	SparkMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SparkMesh->SetCastShadow(false);
	SparkMesh->SetMobility(EComponentMobility::Movable);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereFinder(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereFinder.Succeeded())
	{
		SphereMesh = SphereFinder.Object;
		SparkMesh->SetStaticMesh(SphereMesh);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MatFinder(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
	if (MatFinder.Succeeded())
	{
		BaseMaterial = MatFinder.Object;
	}

	SparkMesh->SetRelativeScale3D(FVector(StartScale));
}

void AHitSpark::BeginPlay()
{
	Super::BeginPlay();
	ApplyColor(SparkColor);
	SetActorScale3D(FVector(StartScale));
}

void AHitSpark::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Age += DeltaTime;
	const float Alpha = FMath::Clamp(Age / FMath::Max(0.01f, Lifetime), 0.f, 1.f);
	// Ease-out scale burst.
	const float Scale = FMath::Lerp(StartScale, EndScale, FMath::InterpEaseOut(0.f, 1.f, Alpha, 2.f));
	SetActorScale3D(FVector(Scale));

	// Fade toward white flash mid-life then dim (via brighter→darker color).
	const FLinearColor Flash = FLinearColor(1.f, 1.f, 0.85f, 1.f);
	const FLinearColor Mid = FMath::Lerp(Flash, SparkColor, FMath::Clamp(Alpha * 2.f, 0.f, 1.f));
	const FLinearColor Final = FMath::Lerp(Mid, SparkColor * 0.3f, FMath::Clamp((Alpha - 0.5f) * 2.f, 0.f, 1.f));
	ApplyColor(Final);

	if (Age >= Lifetime)
	{
		Destroy();
	}
}

void AHitSpark::Configure(const FLinearColor& Color, float InLifetime)
{
	SparkColor = Color;
	Lifetime = InLifetime;
	Age = 0.f;
	ApplyColor(Color);
	SetActorScale3D(FVector(StartScale));
}

void AHitSpark::ApplyColor(const FLinearColor& Color)
{
	if (!SparkMesh || !BaseMaterial)
	{
		return;
	}
	UMaterialInstanceDynamic* DynMat = SparkMesh->CreateDynamicMaterialInstance(0, BaseMaterial);
	if (DynMat)
	{
		DynMat->SetVectorParameterValue(TEXT("Color"), Color);
		DynMat->SetVectorParameterValue(TEXT("BaseColor"), Color);
	}
}

AHitSpark* AHitSpark::SpawnHitSpark(UObject* WorldContextObject, const FVector& Location, const FLinearColor& Color)
{
	UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
	if (!World)
	{
		return nullptr;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AHitSpark* Spark = World->SpawnActor<AHitSpark>(AHitSpark::StaticClass(), Location, FRotator::ZeroRotator, Params);
	if (Spark)
	{
		Spark->Configure(Color, 0.25f);
	}
	return Spark;
}
