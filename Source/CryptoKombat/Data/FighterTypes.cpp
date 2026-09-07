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


FFighterDefinition FCryptoRosterFactory::MakeSolFlash()
{
	FFighterDefinition F;
	F.FighterId = TEXT("SolFlash");
	F.DisplayName = NSLOCTEXT("CryptoKombat", "SolName", "Sol Flash");
	F.ArchetypeTagline = NSLOCTEXT("CryptoKombat", "SolTag", "The Parallel Striker");
	F.Bio = NSLOCTEXT("CryptoKombat", "SolBio",
		"A blur of parallel fists. Confirms blocks before the opponent finishes blinking. Prefers sub-second rounds and overclocked footwear.");
	F.AccentColor = FLinearColor(0.55f, 0.25f, 0.95f);
	F.MaxHealth = 900.f;
	F.WalkSpeed = 520.f;

	F.Normals = {
		MakeMove(TEXT("SF_LP"), NSLOCTEXT("CryptoKombat", "SF_LP", "Slot Jab"),
			NSLOCTEXT("CryptoKombat", "SF_LP_D", "Ultra-fast multi-hit jab."), EAttackSlot::LightPunch, 22.f, 0.18f, 0.12f, 0.f),
		MakeMove(TEXT("SF_HP"), NSLOCTEXT("CryptoKombat", "SF_HP", "Validator Smash"),
			NSLOCTEXT("CryptoKombat", "SF_HP_D", "Quick heavy that chains into itself."), EAttackSlot::HeavyPunch, 55.f, 0.35f, 0.25f, 0.f),
		MakeMove(TEXT("SF_LK"), NSLOCTEXT("CryptoKombat", "SF_LK", "Throughput Kick"),
			NSLOCTEXT("CryptoKombat", "SF_LK_D", "Staccato low multi-hit kick."), EAttackSlot::LightKick, 26.f, 0.2f, 0.14f, 0.f),
		MakeMove(TEXT("SF_HK"), NSLOCTEXT("CryptoKombat", "SF_HK", "Parallel Roundhouse"),
			NSLOCTEXT("CryptoKombat", "SF_HK_D", "Fast advancing kick with bonus hits."), EAttackSlot::HeavyKick, 68.f, 0.4f, 0.3f, 0.f, 95.f),
	};

	F.Specials = {
		MakeMove(TEXT("SF_SP1"), NSLOCTEXT("CryptoKombat", "SF_SP1", "Slot Storm"),
			NSLOCTEXT("CryptoKombat", "SF_SP1_D", "Flurry of parallel punches that fill the lane."), EAttackSlot::Special1, 95.f, 0.45f, 0.4f, 20.f, 100.f),
		MakeMove(TEXT("SF_SP2"), NSLOCTEXT("CryptoKombat", "SF_SP2", "Priority Fee Rush"),
			NSLOCTEXT("CryptoKombat", "SF_SP2_D", "Paid-priority dash that outspeeds everything."), EAttackSlot::Special2, 70.f, 0.35f, 0.28f, 25.f, 130.f),
	};

	FFatalityConcept Fat;
	Fat.FatalityId = TEXT("SF_FAT_Congestion");
	Fat.DisplayName = NSLOCTEXT("CryptoKombat", "SF_FAT", "Network Congestion Crush");
	Fat.FlavorText = NSLOCTEXT("CryptoKombat", "SF_FAT_F",
		"Flood the arena with phantom transactions until the rival freezes mid-frame, then shatter the backlog.");
	Fat.InputHint = TEXT("Forward, Forward, Forward, Light Punch (near)");
	F.Fatalities = { Fat };
	return F;
}

FFighterDefinition FCryptoRosterFactory::MakeDotWeaver()
{
	FFighterDefinition F;
	F.FighterId = TEXT("DotWeaver");
	F.DisplayName = NSLOCTEXT("CryptoKombat", "DotName", "Dot Weaver");
	F.ArchetypeTagline = NSLOCTEXT("CryptoKombat", "DotTag", "The Interchain Architect");
	F.Bio = NSLOCTEXT("CryptoKombat", "DotBio",
		"Threads parachains across the stage like tripwire. Wins by setup: bind, kite, then gavel. Speaks in governance proposals mid-match.");
	F.AccentColor = FLinearColor(0.9f, 0.2f, 0.55f);
	F.MaxHealth = 1000.f;
	F.WalkSpeed = 440.f;

	F.Normals = {
		MakeMove(TEXT("DW_LP"), NSLOCTEXT("CryptoKombat", "DW_LP", "Thread Jab"),
			NSLOCTEXT("CryptoKombat", "DW_LP_D", "Long-reach probing jab."), EAttackSlot::LightPunch, 28.f, 0.26f, 0.2f, 0.f, 95.f),
		MakeMove(TEXT("DW_HP"), NSLOCTEXT("CryptoKombat", "DW_HP", "Governance Punch"),
			NSLOCTEXT("CryptoKombat", "DW_HP_D", "Measured heavy that opens setups."), EAttackSlot::HeavyPunch, 72.f, 0.48f, 0.38f, 0.f),
		MakeMove(TEXT("DW_LK"), NSLOCTEXT("CryptoKombat", "DW_LK", "Collator Sweep"),
			NSLOCTEXT("CryptoKombat", "DW_LK_D", "Low zoning kick."), EAttackSlot::LightKick, 33.f, 0.28f, 0.22f, 0.f, 90.f),
		MakeMove(TEXT("DW_HK"), NSLOCTEXT("CryptoKombat", "DW_HK", "Bridge Kick"),
			NSLOCTEXT("CryptoKombat", "DW_HK_D", "Space-controlling roundhouse."), EAttackSlot::HeavyKick, 78.f, 0.5f, 0.4f, 0.f, 110.f),
	};

	F.Specials = {
		MakeMove(TEXT("DW_SP1"), NSLOCTEXT("CryptoKombat", "DW_SP1", "Parachain Bind"),
			NSLOCTEXT("CryptoKombat", "DW_SP1_D", "Projectile thread that slows and tags for setups."), EAttackSlot::Special1, 80.f, 0.55f, 0.45f, 25.f, 160.f),
		MakeMove(TEXT("DW_SP2"), NSLOCTEXT("CryptoKombat", "DW_SP2", "Relay Kick"),
			NSLOCTEXT("CryptoKombat", "DW_SP2_D", "Advancing kick that shares damage across a bind."), EAttackSlot::Special2, 100.f, 0.5f, 0.4f, 20.f, 90.f),
	};

	FFatalityConcept Fat;
	Fat.FatalityId = TEXT("DW_FAT_Finality");
	Fat.DisplayName = NSLOCTEXT("CryptoKombat", "DW_FAT", "Finality Gavel");
	Fat.FlavorText = NSLOCTEXT("CryptoKombat", "DW_FAT_F",
		"Call a unanimous session vote; the gavel drops and the rival is sealed into a finalized block forever.");
	Fat.InputHint = TEXT("Down, Back, Forward, Heavy Punch (near)");
	F.Fatalities = { Fat };
	return F;
}

FFighterDefinition FCryptoRosterFactory::MakeHaydenSwap()
{
	FFighterDefinition F;
	F.FighterId = TEXT("HaydenSwap");
	F.DisplayName = NSLOCTEXT("CryptoKombat", "HaydenName", "Hayden Swap");
	F.ArchetypeTagline = NSLOCTEXT("CryptoKombat", "HaydenTag", "The Liquidity Ghost");
	F.Bio = NSLOCTEXT("CryptoKombat", "HaydenBio",
		"Phases through mixups like an AMM curve. Leaves pools where you stood. Always somehow long and short at the same time.");
	F.AccentColor = FLinearColor(0.98f, 0.4f, 0.75f);
	F.MaxHealth = 960.f;
	F.WalkSpeed = 490.f;

	F.Normals = {
		MakeMove(TEXT("HS_LP"), NSLOCTEXT("CryptoKombat", "HS_LP", "Swap Jab"),
			NSLOCTEXT("CryptoKombat", "HS_LP_D", "Ambiguous high/low tick jab."), EAttackSlot::LightPunch, 29.f, 0.22f, 0.15f, 0.f),
		MakeMove(TEXT("HS_HP"), NSLOCTEXT("CryptoKombat", "HS_HP", "Curve Punch"),
			NSLOCTEXT("CryptoKombat", "HS_HP_D", "Bent-trajectory heavy for mixups."), EAttackSlot::HeavyPunch, 68.f, 0.4f, 0.3f, 0.f),
		MakeMove(TEXT("HS_LK"), NSLOCTEXT("CryptoKombat", "HS_LK", "Slippage Kick"),
			NSLOCTEXT("CryptoKombat", "HS_LK_D", "Low kick that slides under guards."), EAttackSlot::LightKick, 34.f, 0.26f, 0.18f, 0.f),
		MakeMove(TEXT("HS_HK"), NSLOCTEXT("CryptoKombat", "HS_HK", "Pool Kick"),
			NSLOCTEXT("CryptoKombat", "HS_HK_D", "Heavy kick that leaves a sticky puddle."), EAttackSlot::HeavyKick, 76.f, 0.46f, 0.35f, 0.f, 95.f),
	};

	F.Specials = {
		MakeMove(TEXT("HS_SP1"), NSLOCTEXT("CryptoKombat", "HS_SP1", "Impermanent Loss"),
			NSLOCTEXT("CryptoKombat", "HS_SP1_D", "Trap zone that drains health while they stand in it."), EAttackSlot::Special1, 60.f, 0.4f, 0.55f, 25.f, 120.f),
		MakeMove(TEXT("HS_SP2"), NSLOCTEXT("CryptoKombat", "HS_SP2", "Flash Loan Dash"),
			NSLOCTEXT("CryptoKombat", "HS_SP2_D", "Borrow speed for one frame, repay with a cross-up."), EAttackSlot::Special2, 85.f, 0.35f, 0.3f, 30.f, 140.f),
	};

	FFatalityConcept Fat;
	Fat.FatalityId = TEXT("HS_FAT_RugPool");
	Fat.DisplayName = NSLOCTEXT("CryptoKombat", "HS_FAT", "Rug the Pool");
	Fat.FlavorText = NSLOCTEXT("CryptoKombat", "HS_FAT_F",
		"Arcade gag: yank the liquidity swimming pool out from under them; rival splashes into a cartoon empty basin (pure parody, not financial advice).");
	Fat.InputHint = TEXT("Back, Down, Forward, Special (near)");
	F.Fatalities = { Fat };
	return F;
}

FFighterDefinition FCryptoRosterFactory::MakeTronBlaze()
{
	FFighterDefinition F;
	F.FighterId = TEXT("TronBlaze");
	F.DisplayName = NSLOCTEXT("CryptoKombat", "TronName", "Tron Blaze");
	F.ArchetypeTagline = NSLOCTEXT("CryptoKombat", "TronTag", "The Showman Chain");
	F.Bio = NSLOCTEXT("CryptoKombat", "TronBio",
		"Enters every round under a spotlight. Moves look like trailers. Never quietly KO'd — always with a camera pan.");
	F.AccentColor = FLinearColor(0.95f, 0.15f, 0.2f);
	F.MaxHealth = 1050.f;
	F.WalkSpeed = 460.f;

	F.Normals = {
		MakeMove(TEXT("TB_LP"), NSLOCTEXT("CryptoKombat", "TB_LP", "Spotlight Jab"),
			NSLOCTEXT("CryptoKombat", "TB_LP_D", "Flashy forward jab."), EAttackSlot::LightPunch, 32.f, 0.26f, 0.18f, 0.f),
		MakeMove(TEXT("TB_HP"), NSLOCTEXT("CryptoKombat", "TB_HP", "Marquee Punch"),
			NSLOCTEXT("CryptoKombat", "TB_HP_D", "Heavy with a sparkler trail."), EAttackSlot::HeavyPunch, 78.f, 0.5f, 0.36f, 0.f),
		MakeMove(TEXT("TB_LK"), NSLOCTEXT("CryptoKombat", "TB_LK", "Encore Kick"),
			NSLOCTEXT("CryptoKombat", "TB_LK_D", "Low kick that demands applause."), EAttackSlot::LightKick, 36.f, 0.28f, 0.2f, 0.f),
		MakeMove(TEXT("TB_HK"), NSLOCTEXT("CryptoKombat", "TB_HK", "BitTorrent Boot"),
			NSLOCTEXT("CryptoKombat", "TB_HK_D", "Heavy kick that seeds hitstun to nearby clones."), EAttackSlot::HeavyKick, 85.f, 0.52f, 0.4f, 0.f, 100.f),
	};

	F.Specials = {
		MakeMove(TEXT("TB_SP1"), NSLOCTEXT("CryptoKombat", "TB_SP1", "Sunbeam Spear"),
			NSLOCTEXT("CryptoKombat", "TB_SP1_D", "Glowing spear projectile across the stage."), EAttackSlot::Special1, 115.f, 0.5f, 0.45f, 25.f, 170.f),
		MakeMove(TEXT("TB_SP2"), NSLOCTEXT("CryptoKombat", "TB_SP2", "Arena Drop"),
			NSLOCTEXT("CryptoKombat", "TB_SP2_D", "Command grab that dunks them into a miniature arena."), EAttackSlot::Special2, 105.f, 0.65f, 0.5f, 30.f, 55.f),
	};

	FFatalityConcept Fat;
	Fat.FatalityId = TEXT("TB_FAT_Broadcast");
	Fat.DisplayName = NSLOCTEXT("CryptoKombat", "TB_FAT", "Eternal Broadcast");
	Fat.FlavorText = NSLOCTEXT("CryptoKombat", "TB_FAT_F",
		"Lock the rival into a looping livestream; ratings spike as they dissolve into pixel confetti.");
	Fat.InputHint = TEXT("Forward, Down, Forward, Heavy Kick (near)");
	F.Fatalities = { Fat };
	return F;
}

FFighterDefinition FCryptoRosterFactory::MakeArthurPerp()
{
	FFighterDefinition F;
	F.FighterId = TEXT("ArthurPerp");
	F.DisplayName = NSLOCTEXT("CryptoKombat", "ArthurName", "Arthur Perp");
	F.ArchetypeTagline = NSLOCTEXT("CryptoKombat", "ArthurTag", "The Leverage King");
	F.Bio = NSLOCTEXT("CryptoKombat", "ArthurBio",
		"Every punch is sized. Wins big or gets carried out on a funding-rate stretcher. Reads the order book like a crystal ball.");
	F.AccentColor = FLinearColor(0.15f, 0.85f, 0.9f);
	F.MaxHealth = 920.f;
	F.WalkSpeed = 500.f;

	F.Normals = {
		MakeMove(TEXT("AP_LP"), NSLOCTEXT("CryptoKombat", "AP_LP", "Tick Jab"),
			NSLOCTEXT("CryptoKombat", "AP_LP_D", "Risky fast jab with bonus on counter."), EAttackSlot::LightPunch, 26.f, 0.2f, 0.14f, 0.f),
		MakeMove(TEXT("AP_HP"), NSLOCTEXT("CryptoKombat", "AP_HP", "Sized Punch"),
			NSLOCTEXT("CryptoKombat", "AP_HP_D", "Heavy that hits harder when you are behind."), EAttackSlot::HeavyPunch, 80.f, 0.45f, 0.34f, 0.f),
		MakeMove(TEXT("AP_LK"), NSLOCTEXT("CryptoKombat", "AP_LK", "Basis Kick"),
			NSLOCTEXT("CryptoKombat", "AP_LK_D", "Low kick that shifts momentum."), EAttackSlot::LightKick, 33.f, 0.25f, 0.18f, 0.f),
		MakeMove(TEXT("AP_HK"), NSLOCTEXT("CryptoKombat", "AP_HK", "Open Interest Kick"),
			NSLOCTEXT("CryptoKombat", "AP_HK_D", "High-reward roundhouse; whiff and pay."), EAttackSlot::HeavyKick, 88.f, 0.5f, 0.42f, 0.f, 100.f),
	};

	F.Specials = {
		MakeMove(TEXT("AP_SP1"), NSLOCTEXT("CryptoKombat", "AP_SP1", "100x Long"),
			NSLOCTEXT("CryptoKombat", "AP_SP1_D", "All-in forward rush; huge damage or huge punish."), EAttackSlot::Special1, 150.f, 0.55f, 0.5f, 35.f, 120.f),
		MakeMove(TEXT("AP_SP2"), NSLOCTEXT("CryptoKombat", "AP_SP2", "Funding Rate Drain"),
			NSLOCTEXT("CryptoKombat", "AP_SP2_D", "Siphon Moon Meter and chip while they hold a position."), EAttackSlot::Special2, 45.f, 0.4f, 0.6f, 20.f, 100.f),
	};

	FFatalityConcept Fat;
	Fat.FatalityId = TEXT("AP_FAT_Liq");
	Fat.DisplayName = NSLOCTEXT("CryptoKombat", "AP_FAT", "Forced Liquidation Finisher");
	Fat.FlavorText = NSLOCTEXT("CryptoKombat", "AP_FAT_F",
		"Margin call cascade: the rival's health bar auto-sells to zero amid flashing orange warnings.");
	Fat.InputHint = TEXT("Down, Forward, Forward, Special (near)");
	F.Fatalities = { Fat };
	return F;
}

FFighterDefinition FCryptoRosterFactory::MakeAdaScholar()
{
	FFighterDefinition F;
	F.FighterId = TEXT("AdaScholar");
	F.DisplayName = NSLOCTEXT("CryptoKombat", "AdaName", "Ada Scholar");
	F.ArchetypeTagline = NSLOCTEXT("CryptoKombat", "AdaTag", "The Peer-Reviewed Pugilist");
	F.Bio = NSLOCTEXT("CryptoKombat", "AdaBio",
		"Measures twice, punches once. Prefers counters backed by lemmas. Will cite sources mid-combo if you ask politely.");
	F.AccentColor = FLinearColor(0.2f, 0.35f, 0.75f);
	F.MaxHealth = 1100.f;
	F.WalkSpeed = 420.f;

	F.Normals = {
		MakeMove(TEXT("AS_LP"), NSLOCTEXT("CryptoKombat", "AS_LP", "Lemma Jab"),
			NSLOCTEXT("CryptoKombat", "AS_LP_D", "Patient, precise jab."), EAttackSlot::LightPunch, 32.f, 0.28f, 0.2f, 0.f),
		MakeMove(TEXT("AS_HP"), NSLOCTEXT("CryptoKombat", "AS_HP", "Theorem Punch"),
			NSLOCTEXT("CryptoKombat", "AS_HP_D", "Heavy that rewards correct spacing."), EAttackSlot::HeavyPunch, 82.f, 0.52f, 0.4f, 0.f),
		MakeMove(TEXT("AS_LK"), NSLOCTEXT("CryptoKombat", "AS_LK", "Citation Kick"),
			NSLOCTEXT("CryptoKombat", "AS_LK_D", "Measured low check."), EAttackSlot::LightKick, 36.f, 0.3f, 0.22f, 0.f),
		MakeMove(TEXT("AS_HK"), NSLOCTEXT("CryptoKombat", "AS_HK", "Haskell Roundhouse"),
			NSLOCTEXT("CryptoKombat", "AS_HK_D", "Functional, pure, and painful."), EAttackSlot::HeavyKick, 86.f, 0.55f, 0.42f, 0.f, 90.f),
	};

	F.Specials = {
		MakeMove(TEXT("AS_SP1"), NSLOCTEXT("CryptoKombat", "AS_SP1", "Formal Proof Guard"),
			NSLOCTEXT("CryptoKombat", "AS_SP1_D", "Counter stance that punishes the next strike."), EAttackSlot::Special1, 0.f, 0.25f, 0.7f, 20.f),
		MakeMove(TEXT("AS_SP2"), NSLOCTEXT("CryptoKombat", "AS_SP2", "Epoch Sweep"),
			NSLOCTEXT("CryptoKombat", "AS_SP2_D", "Wide low sweep that resets the round's pacing."), EAttackSlot::Special2, 95.f, 0.5f, 0.45f, 25.f, 100.f),
	};

	FFatalityConcept Fat;
	Fat.FatalityId = TEXT("AS_FAT_Reject");
	Fat.DisplayName = NSLOCTEXT("CryptoKombat", "AS_FAT", "Peer Review Rejection");
	Fat.FlavorText = NSLOCTEXT("CryptoKombat", "AS_FAT_F",
		"Stamp REJECTED across the rival in glowing ink; they crumple into a stack of unread whitepapers.");
	Fat.InputHint = TEXT("Back, Back, Down, Heavy Punch (near)");
	F.Fatalities = { Fat };
	return F;
}

TArray<FFighterDefinition> FCryptoRosterFactory::MakeDefaultRoster()
{
	return {
		MakeSatoshiShadow(),
		MakeVitalSpark(),
		MakeCZChain(),
		MakeBrianCoin(),
		MakeSolFlash(),
		MakeDotWeaver(),
		MakeHaydenSwap(),
		MakeTronBlaze(),
		MakeArthurPerp(),
		MakeAdaScholar()
	};
}
