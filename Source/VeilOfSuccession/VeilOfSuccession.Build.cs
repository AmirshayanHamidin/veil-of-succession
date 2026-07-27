// Veil of Succession - fictional political thriller. All characters fictitious.
using UnrealBuildTool;

public class VeilOfSuccession : ModuleRules
{
	public VeilOfSuccession(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core", "CoreUObject", "Engine", "InputCore",
			"EnhancedInput", "AIModule", "GameplayTasks", "NavigationSystem"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
	}
}
