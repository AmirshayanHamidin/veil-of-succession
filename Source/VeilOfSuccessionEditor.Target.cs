// Veil of Succession - fictional political thriller. All characters fictitious.
using UnrealBuildTool;
using System.Collections.Generic;

public class VeilOfSuccessionEditorTarget : TargetRules
{
	public VeilOfSuccessionEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("VeilOfSuccession");
	}
}
