// Copyright CryptoKombat. Phase 1 scaffold.

using UnrealBuildTool;
using System.Collections.Generic;

public class CryptoKombatTarget : TargetRules
{
	public CryptoKombatTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("CryptoKombat");
	}
}
