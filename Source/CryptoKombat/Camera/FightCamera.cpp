// Copyright CryptoKombat. Phase 1 scaffold.

#include "Camera/FightCamera.h"
#include "Characters/CryptoFighter.h"
#include "Camera/CameraComponent.h"

AFightCamera::AFightCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	if (UCameraComponent* Cam = GetCameraComponent())
	{
		Cam->bConstrainAspectRatio = false;
		Cam->SetProjectionMode(bUseOrtho ? ECameraProjectionMode::Orthographic : ECameraProjectionMode::Perspective);
		Cam->SetOrthoWidth(900.f);
		Cam->SetFieldOfView(50.f);
	}
}

void AFightCamera::SetTrackedFighters(ACryptoFighter* InP1, ACryptoFighter* InP2)
{
	P1 = InP1;
	P2 = InP2;
}

void AFightCamera::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!P1 || !P2)
	{
		return;
	}

	const FVector L1 = P1->GetActorLocation();
	const FVector L2 = P2->GetActorLocation();
	const float MidY = (L1.Y + L2.Y) * 0.5f;
	const float MidZ = (L1.Z + L2.Z) * 0.5f + HeightOffsetZ;
	const float Separation = FMath::Abs(L1.Y - L2.Y);

	const FVector TargetLoc(CameraDistanceX, MidY, MidZ);
	const FVector NewLoc = FMath::VInterpTo(GetActorLocation(), TargetLoc, DeltaSeconds, InterpSpeed);
	SetActorLocation(NewLoc);
	SetActorRotation(FRotator(0.f, 0.f, 0.f)); // look +X toward stage? Stage along Y, camera on -X looking at origin
	// Face +X so we look toward the fighters' X≈0 plane from negative X.
	SetActorRotation(FRotator(0.f, 0.f, 0.f));

	if (UCameraComponent* Cam = GetCameraComponent())
	{
		if (bUseOrtho)
		{
			Cam->SetProjectionMode(ECameraProjectionMode::Orthographic);
			const float DesiredWidth = FMath::Clamp(Separation + Padding * 2.f, MinOrthoWidth, MaxOrthoWidth);
			const float Current = Cam->OrthoWidth;
			Cam->SetOrthoWidth(FMath::FInterpTo(Current, DesiredWidth, DeltaSeconds, InterpSpeed));
		}
	}
}
