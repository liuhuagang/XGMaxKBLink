// Copyright 2025 Xiao Gang. All Rights Reserved.

using UnrealBuildTool;

public class XGMaxKBDemo : ModuleRules
{
	public XGMaxKBDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });


	}
}
