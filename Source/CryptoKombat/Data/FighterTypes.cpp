// Copyright CryptoKombat. Phase 1 scaffold.

#include "Data/FighterTypes.h"

static FMoveDefinition MakeMove(
	const FName Id,
	const FText& Name,
	const FText& Desc,
	EAttackSlot Slot,
	float Damage,
	float Hitstun,
	float Active,
	float MoonCost,
	float ForwardOffset = 80.f)
{
	FMoveDefinition M;
	M.MoveId = Id;
	M.DisplayName = Name;
	M.Description = Desc;
	M.Slot = Slot;
	M.Damage = Damage;
	M.HitstunDuration = Hitstun;
	M.ActiveDuration = Active;
	M.MoonCost = MoonCost;
	M.TraceForwardOffset = ForwardOffset;
	return M;
}

FFighterDefinition FCryptoRosterFactory::MakeSatoshiShadow()
{
	FFighterDefinition F;
	F.FighterId = TEXT("SatoshiShadow");
	F.DisplayName = NSLOCTEXT("CryptoKombat", "SatoshiName", "Satoshi Shadow");
	F.ArchetypeTagline = NSLOCTEXT("CryptoKombat", "SatoshiTag", "The Anonymous Genesis");
	F.Bio = NSLOCTEXT("CryptoKombat", "SatoshiBio",
		"A hooded figure woven from whitepaper mist. Speaks only in hashes. Nobody knows if they ever existed — until the first punch lands.");
	F.AccentColor = FLinearColor(0.95f, 0.55f, 0.1f);
	F.MaxHealth = 1000.f;
	F.WalkSpeed = 420.f;

	F.Normals = {
		MakeMove(TEXT("SS_LP"), NSLOCTEXT("CryptoKombat", "SS_LP", "Hash Jab"),
			NSLOCTEXT("CryptoKombat", "SS_LP_D", "Quick forward jab."), EAttackSlot::LightPunch, 30.f, 0.25f, 0.18f, 0.f),
		MakeMove(TEXT("SS_HP"), NSLOCTEXT("CryptoKombat", "SS_HP", "Block Reward"),
			NSLOCTEXT("CryptoKombat", "SS_HP_D", "Heavy overhead smash."), EAttackSlot::HeavyPunch, 70.f, 0.45f, 0.35f, 0.f),
		MakeMove(TEXT("SS_LK"), NSLOCTEXT("CryptoKombat", "SS_LK", "Nonce Kick"),
			NSLOCTEXT("CryptoKombat", "SS_LK_D", "Low probing kick."), EAttackSlot::LightKick, 35.f, 0.28f, 0.2f, 0.f),
		MakeMove(TEXT("SS_HK"), NSLOCTEXT("CryptoKombat", "SS_HK", "Difficulty Adjustment"),
			NSLOCTEXT("CryptoKombat", "SS_HK_D", "Launching roundhouse."), EAttackSlot::HeavyKick, 80.f, 0.5f, 0.4f, 0.f, 90.f),
	};

	F.Specials = {
		MakeMove(TEXT("SS_SP1"), NSLOCTEXT("CryptoKombat", "SS_SP1", "Genesis Pulse"),
			NSLOCTEXT("CryptoKombat", "SS_SP1_D", "Forward energy wave of primal blocks."), EAttackSlot::Special1, 120.f, 0.55f, 0.45f, 25.f, 140.f),
		MakeMove(TEXT("SS_SP2"), NSLOCTEXT("CryptoKombat", "SS_SP2", "HODL Counter"),
			NSLOCTEXT("CryptoKombat", "SS_SP2_D", "Parry that converts damage into Moon Meter."), EAttackSlot::Special2, 0.f, 0.2f, 0.6f, 15.f),
	};

	FFatalityConcept Fat;
	Fat.FatalityId = TEXT("SS_FAT_Rugpull");
	Fat.DisplayName = NSLOCTEXT("CryptoKombat", "SS_FAT", "Rugpull Rift");
	Fat.FlavorText = NSLOCTEXT("CryptoKombat", "SS_FAT_F",
		"Yank the arena floor like a liquidity rug; rival falls into an endless mempool abyss.");
	Fat.InputHint = TEXT("Down, Down, Forward, Heavy Punch (near)");
	F.Fatalities = { Fat };
	return F;
}

FFighterDefinition FCryptoRosterFactory::MakeVitalSpark()
{
	FFighterDefinition F;
	F.FighterId = TEXT("VitalSpark");
	F.DisplayName = NSLOCTEXT("CryptoKombat", "VitalName", "Vital Spark");
	F.ArchetypeTagline = NSLOCTEXT("CryptoKombat", "VitalTag", "The Smart-Contract Sage");
	F.Bio = NSLOCTEXT("CryptoKombat", "VitalBio",
		"A lanky coder-monk whose fists compile mid-combo. Prefers gas-efficient strikes and philosophical mid-match tweets (imaginary ones).");
	F.AccentColor = FLinearColor(0.4f, 0.55f, 0.95f);
	F.MaxHealth = 950.f;
	F.WalkSpeed = 480.f;

	F.Normals = {
		MakeMove(TEXT("VS_LP"), NSLOCTEXT("CryptoKombat", "VS_LP", "Gas Jab"),
			NSLOCTEXT("CryptoKombat", "VS_LP_D", "Cheap, fast poke."), EAttackSlot::LightPunch, 28.f, 0.22f, 0.15f, 0.f),
		MakeMove(TEXT("VS_HP"), NSLOCTEXT("CryptoKombat", "VS_HP", "Opcode Smash"),
			NSLOCTEXT("CryptoKombat", "VS_HP_D", "Heavy contract punch."), EAttackSlot::HeavyPunch, 65.f, 0.42f, 0.32f, 0.f),
		MakeMove(TEXT("VS_LK"), NSLOCTEXT("CryptoKombat", "VS_LK", "Shard Sweep"),
			NSLOCTEXT("CryptoKombat", "VS_LK_D", "Low sweep."), EAttackSlot::LightKick, 32.f, 0.26f, 0.2f, 0.f),
		MakeMove(TEXT("VS_HK"), NSLOCTEXT("CryptoKombat", "VS_HK", "Beacon Kick"),
			NSLOCTEXT("CryptoKombat", "VS_HK_D", "Long-range kick."), EAttackSlot::HeavyKick, 75.f, 0.48f, 0.38f, 0.f, 100.f),
	};

	F.Specials = {
		MakeMove(TEXT("VS_SP1"), NSLOCTEXT("CryptoKombat", "VS_SP1", "Liquidation Laser"),
			NSLOCTEXT("CryptoKombat", "VS_SP1_D", "Beam that deletes overleveraged health."), EAttackSlot::Special1, 130.f, 0.5f, 0.5f, 30.f, 160.f),
		MakeMove(TEXT("VS_SP2"), NSLOCTEXT("CryptoKombat", "VS_SP2", "Fork Flip"),
			NSLOCTEXT("CryptoKombat", "VS_SP2_D", "Teleport-style hard fork reposition."), EAttackSlot::Special2, 40.f, 0.3f, 0.35f, 20.f),
	};

	FFatalityConcept Fat;
	Fat.FatalityId = TEXT("VS_FAT_Reentrancy");
	Fat.DisplayName = NSLOCTEXT("CryptoKombat", "VS_FAT", "Reentrancy Ruin");
	Fat.FlavorText = NSLOCTEXT("CryptoKombat", "VS_FAT_F",
		"Trap the foe in a recursive call stack until their sprite overflows into static.");
	Fat.InputHint = TEXT("Back, Forward, Back, Special (near)");
	F.Fatalities = { Fat };
	return F;
}

FFighterDefinition FCryptoRosterFactory::MakeCZChain()
{
	FFighterDefinition F;
	F.FighterId = TEXT("CZChain");
	F.DisplayName = NSLOCTEXT("CryptoKombat", "CZName", "CZ Chain");
	F.ArchetypeTagline = NSLOCTEXT("CryptoKombat", "CZTag", "The Exchange Enforcer");
	F.Bio = NSLOCTEXT("CryptoKombat", "CZBio",
		"Built like a vault door. Speaks in four-word sentences. Funds never leave — fighters sometimes do.");
	F.AccentColor = FLinearColor(0.95f, 0.85f, 0.2f);
	F.MaxHealth = 1100.f;
	F.WalkSpeed = 400.f;

	F.Normals = {
		MakeMove(TEXT("CZ_LP"), NSLOCTEXT("CryptoKombat", "CZ_LP", "Listing Jab"),
			NSLOCTEXT("CryptoKombat", "CZ_LP_D", "Stiff forward jab."), EAttackSlot::LightPunch, 35.f, 0.28f, 0.2f, 0.f),
		MakeMove(TEXT("CZ_HP"), NSLOCTEXT("CryptoKombat", "CZ_HP", "Cold Wallet Crush"),
			NSLOCTEXT("CryptoKombat", "CZ_HP_D", "Two-handed slam."), EAttackSlot::HeavyPunch, 85.f, 0.55f, 0.4f, 0.f),
		MakeMove(TEXT("CZ_LK"), NSLOCTEXT("CryptoKombat", "CZ_LK", "Fee Kick"),
			NSLOCTEXT("CryptoKombat", "CZ_LK_D", "Takes a cut of their stance."), EAttackSlot::LightKick, 38.f, 0.3f, 0.22f, 0.f),
		MakeMove(TEXT("CZ_HK"), NSLOCTEXT("CryptoKombat", "CZ_HK", "Margin Call"),
			NSLOCTEXT("CryptoKombat", "CZ_HK_D", "Heavy advancing kick."), EAttackSlot::HeavyKick, 90.f, 0.55f, 0.42f, 0.f, 85.f),
	};

	F.Specials = {
		MakeMove(TEXT("CZ_SP1"), NSLOCTEXT("CryptoKombat", "CZ_SP1", "Withdrawal Lock"),
			NSLOCTEXT("CryptoKombat", "CZ_SP1_D", "Grab that freezes the opponent mid-combo."), EAttackSlot::Special1, 100.f, 0.7f, 0.5f, 25.f, 60.f),
		MakeMove(TEXT("CZ_SP2"), NSLOCTEXT("CryptoKombat", "CZ_SP2", "SAFU Shield"),
			NSLOCTEXT("CryptoKombat", "CZ_SP2_D", "Armored advance that shrugs chip damage."), EAttackSlot::Special2, 50.f, 0.35f, 0.55f, 20.f),
	};

	FFatalityConcept Fat;
	Fat.FatalityId = TEXT("CZ_FAT_Liquidation");
	Fat.DisplayName = NSLOCTEXT("CryptoKombat", "CZ_FAT", "Total Liquidation");
	Fat.FlavorText = NSLOCTEXT("CryptoKombat", "CZ_FAT_F",
		"Mark-to-market the rival to zero; they shatter into yellow warning candles.");
	Fat.InputHint = TEXT("Forward, Forward, Down, Heavy Kick (near)");
	F.Fatalities = { Fat };
	return F;
}

FFighterDefinition FCryptoRosterFactory::MakeBrianCoin()
{
	FFighterDefinition F;
	F.FighterId = TEXT("BrianCoin");
	F.DisplayName = NSLOCTEXT("CryptoKombat", "BrianName", "Brian Coin");
	F.ArchetypeTagline = NSLOCTEXT("CryptoKombat", "BrianTag", "The Meme-Market Maverick");
	F.Bio = NSLOCTEXT("CryptoKombat", "BrianBio",
		"Surfs volatility like a longboard. Wins fights with vibes, volume, and suspiciously timed tweets from an alternate timeline.");
	F.AccentColor = FLinearColor(0.2f, 0.9f, 0.55f);
	F.MaxHealth = 980.f;
	F.WalkSpeed = 500.f;

	F.Normals = {
		MakeMove(TEXT("BC_LP"), NSLOCTEXT("CryptoKombat", "BC_LP", "Pump Jab"),
			NSLOCTEXT("CryptoKombat", "BC_LP_D", "Snappy high jab."), EAttackSlot::LightPunch, 30.f, 0.24f, 0.16f, 0.f),
		MakeMove(TEXT("BC_HP"), NSLOCTEXT("CryptoKombat", "BC_HP", "Volume Punch"),
			NSLOCTEXT("CryptoKombat", "BC_HP_D", "Heavy that gains damage mid-combo."), EAttackSlot::HeavyPunch, 70.f, 0.4f, 0.3f, 0.f),
		MakeMove(TEXT("BC_LK"), NSLOCTEXT("CryptoKombat", "BC_LK", "Dip Kick"),
			NSLOCTEXT("CryptoKombat", "BC_LK_D", "Low kick into a buy-the-dip hop."), EAttackSlot::LightKick, 34.f, 0.27f, 0.2f, 0.f),
		MakeMove(TEXT("BC_HK"), NSLOCTEXT("CryptoKombat", "BC_HK", "Moon Kick"),
			NSLOCTEXT("CryptoKombat", "BC_HK_D", "Upward kick that fills Moon Meter on hit."), EAttackSlot::HeavyKick, 78.f, 0.48f, 0.36f, 0.f, 95.f),
	};

	F.Specials = {
		MakeMove(TEXT("BC_SP1"), NSLOCTEXT("CryptoKombat", "BC_SP1", "To The Moon"),
			NSLOCTEXT("CryptoKombat", "BC_SP1_D", "Rising launcher trailing green candles."), EAttackSlot::Special1, 110.f, 0.6f, 0.45f, 25.f, 70.f),
		MakeMove(TEXT("BC_SP2"), NSLOCTEXT("CryptoKombat", "BC_SP2", "Whale Splash"),
			NSLOCTEXT("CryptoKombat", "BC_SP2_D", "Fullscreen splash that pushes both fighters."), EAttackSlot::Special2, 90.f, 0.45f, 0.5f, 35.f, 120.f),
	};

	FFatalityConcept Fat;
	Fat.FatalityId = TEXT("BC_FAT_Dump");
	Fat.DisplayName = NSLOCTEXT("CryptoKombat", "BC_FAT", "Exit Liquidity");
	Fat.FlavorText = NSLOCTEXT("CryptoKombat", "BC_FAT_F",
		"Sell the bags mid-air; rival is crushed under a cascading red chart.");
	Fat.InputHint = TEXT("Down, Forward, Down, Special (near)");
	F.Fatalities = { Fat };
	return F;
}

TArray<FFighterDefinition> FCryptoRosterFactory::MakeDefaultRoster()
{
	return {
		MakeSatoshiShadow(),
		MakeVitalSpark(),
		MakeCZChain(),
		MakeBrianCoin()
	};
}
