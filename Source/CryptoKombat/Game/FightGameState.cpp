// Copyright CryptoKombat. Phase 1 scaffold.

#include "Game/FightGameState.h"

AFightGameState::AFightGameState()
{
	PrimaryActorTick.bCanEverTick = false;
	RoundTimeRemaining = RoundDurationSeconds;
}

void AFightGameState::ResetRoundClock()
{
	RoundTimeRemaining = RoundDurationSeconds;
}

void AFightGameState::TickRoundClock(float DeltaSeconds)
{
	if (RoundPhase != ERoundPhase::Fighting)
	{
		return;
	}
	RoundTimeRemaining = FMath::Max(0.f, RoundTimeRemaining - DeltaSeconds);
}

bool AFightGameState::IsMatchOver() const
{
	return P1RoundsWon >= RoundsToWin || P2RoundsWon >= RoundsToWin || RoundPhase == ERoundPhase::MatchEnd;
}
