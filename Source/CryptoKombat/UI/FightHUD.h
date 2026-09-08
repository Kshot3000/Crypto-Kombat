// Copyright CryptoKombat. Phase 1 scaffold.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FightHUD.generated.h"

class ACryptoFighter;
class AFightGameState;

/**
 * Neon arcade fight HUD drawn with Canvas (no UMG assets required).
 * P1/P2 names, health, Moon Meter, round timer, round score.
 */
UCLASS()
class CRYPTOKOMBAT_API AFightHUD : public AHUD
{
	GENERATED_BODY()

public:
	AFightHUD();

	virtual void DrawHUD() override;

protected:
	void ResolveFighters();
	void DrawBar(float X, float Y, float W, float H, float Percent, const FLinearColor& Fill, const FLinearColor& Back, bool bRightAlign = false);
	void DrawCenteredText(const FString& Text, float CenterX, float Y, const FLinearColor& Color, float Scale = 1.2f);

	UPROPERTY()
	TObjectPtr<ACryptoFighter> CachedP1;

	UPROPERTY()
	TObjectPtr<ACryptoFighter> CachedP2;

	/** Neon palette fallbacks when fighter accents unavailable. */
	FLinearColor P1BarColor = FLinearColor(0.15f, 0.95f, 1.f, 1.f);
	FLinearColor P2BarColor = FLinearColor(1.f, 0.2f, 0.85f, 1.f);
	FLinearColor MoonColor = FLinearColor(0.85f, 0.75f, 1.f, 1.f);
	FLinearColor BarBack = FLinearColor(0.02f, 0.02f, 0.05f, 0.85f);
	FLinearColor TextWhite = FLinearColor(0.95f, 0.95f, 1.f, 1.f);
};
