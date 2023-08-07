using UnrealBuildTool;

public class U05_Session : ModuleRules
{
	public U05_Session(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.Add(ModuleDirectory);

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"HeadMountedDisplay",
			"UMG",
			"OnlineSubSystem",
			"OnlineSubSystemSteam"
		});
	}
}
