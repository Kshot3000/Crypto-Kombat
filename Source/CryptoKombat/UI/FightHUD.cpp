// Copyright CryptoKombat. Phase 1 scaffold.

#include "UI/FightHUD.h"
#include "Characters/CryptoFighter.h"
#include "Game/FightGameState.h"
#include "Game/FightGameMode.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

AFightHUD::AFightHUD()
{
}

void AFightHUD::ResolveFighters()
{
	if (CachedP1 && CachedP2)
	{
		return;
	}

	if (AFightGameMode* GM = Cast<AFightGameMode>(GetWorld() ? GetWorld()->GetAuthGameMode() : nullptr))
	{
		CachedP1 = GM->GetP1Fighter();
		CachedP2 = GM->GetP2Fighter();
		return;
	}

	// Fallback: find CryptoFighter pawns by PlayerIndex.
	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACryptoFighter::StaticClass(), Found);
	for (AActor* A : Found)
	{
		if (ACryptoFighter* F = Cast<ACryptoFighter>(A))
		{
			if (F->PlayerIndex == 0)
			{
				CachedP1 = F;
			}
			else if (F->PlayerIndex == 1)
			{
				CachedP2 = F;
			}
		}
	}
}

void AFightHUD::DrawBar(float X, float Y, float W, float H, float Percent, const FLinearColor& Fill, const FLinearColor& Back, bool bRightAlign)
{
	if (!Canvas)
	{
		return;
	}

	Percent = FMath::Clamp(Percent, 0.f, 1.f);
	FCanvasTileItem BackTile(FVector2D(X, Y), FVector2D(W, H), Back);
	BackTile.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(BackTile);

	const float FillW = W * Percent;
	const float FillX = bRightAlign ? (X + W - FillW) : X;
	if (FillW > 0.5f)
	{
		FCanvasTileItem FillTile(FVector2D(FillX, Y), FVector2D(FillW, H), Fill);
		FillTile.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(FillTile);
	}

	// Thin neon border (top + bottom edges).
	const FLinearColor Edge(Fill.R, Fill.G, Fill.B, 0.55f);
	FCanvasTileItem Top(FVector2D(X, Y), FVector2D(W, 2.f), Edge);
	Top.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(Top);
	FCanvasTileItem Bot(FVector2D(X, Y + H - 2.f), FVector2D(W, 2.f), Edge);
	Bot.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(Bot);
}

void AFightHUD::DrawCenteredText(const FString& Text, float CenterX, float Y, const FLinearColor& Color, float Scale)
{
	if (!Canvas)
	{
		return;
	}
	float TW = 0.f, TH = 0.f;
	GetTextSize(Text, TW, TH, GEngine->GetLargeFont(), Scale);
	const FVector2D Pos(CenterX - TW * 0.5f, Y);
	FCanvasTextItem Item(Pos, FText::FromString(Text), GEngine->GetLargeFont(), Color);
	Item.Scale = FVector2D(Scale, Scale);
	Item.bOutlined = true;
	Item.OutlineColor = FLinearColor(0.f, 0.f, 0.f, 0.9f);
	Canvas->DrawItem(Item);
}

void AFightHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!Canvas)
	{
		return;
	}

	ResolveFighters();

	const float SW = Canvas->SizeX;
	const float SH = Canvas->SizeY;
	const float Margin = SW * 0.04f;
	const float BarW = SW * 0.32f;
	const float HealthH = FMath::Max(14.f, SH * 0.028f);
	const float MoonH = FMath::Max(8.f, SH * 0.012f);
	const float TopY = SH * 0.04f;

	AFightGameState* GS = GetWorld() ? GetWorld()->GetGameState<AFightGameState>() : nullptr;

	const FString P1Name = CachedP1 ? CachedP1->FighterData.DisplayName.ToString() : TEXT("P1");
	const FString P2Name = CachedP2 ? CachedP2->FighterData.DisplayName.ToString() : TEXT("P2");
	const float P1Health = CachedP1 ? CachedP1->GetHealthPercent() : 0.f;
	const float P2Health = CachedP2 ? CachedP2->GetHealthPercent() : 0.f;
	const float P1Moon = CachedP1 ? CachedP1->GetMoonPercent() : 0.f;
	const float P2Moon = CachedP2 ? CachedP2->GetMoonPercent() : 0.f;

	FLinearColor P1Fill = CachedP1 ? CachedP1->FighterData.AccentColor : P1BarColor;
	FLinearColor P2Fill = CachedP2 ? CachedP2->FighterData.AccentColor : P2BarColor;
	P1Fill.A = 1.f;
	P2Fill.A = 1.f;

	// Names
	{
		float TW = 0.f, TH = 0.f;
		GetTextSize(P1Name, TW, TH, GEngine->GetMediumFont(), 1.1f);
		FCanvasTextItem N1(FVector2D(Margin, TopY - TH - 4.f), FText::FromString(P1Name), GEngine->GetMediumFont(), TextWhite);
		N1.Scale = FVector2D(1.1f, 1.1f);
		N1.bOutlined = true;
		N1.OutlineColor = FLinearColor(0.f, 0.f, 0.f, 0.85f);
		Canvas->DrawItem(N1);

		GetTextSize(P2Name, TW, TH, GEngine->GetMediumFont(), 1.1f);
		FCanvasTextItem N2(FVector2D(SW - Margin - TW, TopY - TH - 4.f), FText::FromString(P2Name), GEngine->GetMediumFont(), TextWhite);
		N2.Scale = FVector2D(1.1f, 1.1f);
		N2.bOutlined = true;
		N2.OutlineColor = FLinearColor(0.f, 0.f, 0.f, 0.85f);
		Canvas->DrawItem(N2);
	}

	// Health bars (P1 L→R, P2 R→L drain)
	DrawBar(Margin, TopY, BarW, HealthH, P1Health, P1Fill, BarBack, false);
	DrawBar(SW - Margin - BarW, TopY, BarW, HealthH, P2Health, P2Fill, BarBack, true);

	// Moon Meter bars under health
	const float MoonY = TopY + HealthH + 6.f;
	DrawBar(Margin, MoonY, BarW * 0.85f, MoonH, P1Moon, MoonColor, BarBack, false);
	DrawBar(SW - Margin - BarW * 0.85f, MoonY, BarW * 0.85f, MoonH, P2Moon, MoonColor, BarBack, true);

	// Round timer (center)
	const int32 Seconds = GS ? FMath::CeilToInt(GS->RoundTimeRemaining) : 99;
	DrawCenteredText(FString::Printf(TEXT("%02d"), Seconds), SW * 0.5f, TopY - 4.f, TextWhite, 1.6f);

	// Round score
	const int32 S1 = GS ? GS->P1RoundsWon : 0;
	const int32 S2 = GS ? GS->P2RoundsWon : 0;
	DrawCenteredText(FString::Printf(TEXT("%d  -  %d"), S1, S2), SW * 0.5f, TopY + HealthH + 10.f, FLinearColor(0.7f, 0.9f, 1.f, 1.f), 1.0f);

	// Phase banner
	if (GS)
	{
		FString Banner;
		switch (GS->RoundPhase)
		{
		case ERoundPhase::PreRound: Banner = TEXT("READY"); break;
		case ERoundPhase::RoundEnd: Banner = TEXT("ROUND OVER"); break;
		case ERoundPhase::MatchEnd: Banner = TEXT("MATCH OVER"); break;
		case ERoundPhase::FinishHim: Banner = TEXT("FINISH THEM"); break;
		default: break;
		}
		if (!Banner.IsEmpty())
		{
			DrawCenteredText(Banner, SW * 0.5f, SH * 0.35f, FLinearColor(1.f, 0.9f, 0.2f, 1.f), 2.0f);
		}
		else if (GS->RoundPhase == ERoundPhase::Fighting && GS->RoundTimeRemaining > GS->RoundDurationSeconds - 0.6f)
		{
			DrawCenteredText(TEXT("FIGHT!"), SW * 0.5f, SH * 0.35f, FLinearColor(0.2f, 1.f, 0.85f, 1.f), 2.2f);
		}
	}
}
