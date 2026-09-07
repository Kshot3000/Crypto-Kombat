// Copyright CryptoKombat. Phase 1 scaffold.

using UnrealBuildTool;
using System.Collections.Generic;

public class CryptoKombatEditorTarget : TargetRules
{
	public CryptoKombatEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("CryptoKombat");
	}
}
