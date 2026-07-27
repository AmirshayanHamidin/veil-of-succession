// Veil of Succession - fictional political thriller. All characters fictitious.
using UnrealBuildTool;
using System.Collections.Generic;

public class VeilOfSuccessionTarget : TargetRules
{
	public VeilOfSuccessionTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("VeilOfSuccession");
	}
}
