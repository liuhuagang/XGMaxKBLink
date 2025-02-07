// Copyright 2025 Xiao Gang. All Rights Reserved.
using UnrealBuildTool;

public class XGMaxKBLink : ModuleRules
{
	public XGMaxKBLink(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {

             "$(ModuleDir)/Public/AsyncAction",
             "$(ModuleDir)/Public/Type",


            }
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {

                "XGMaxKBLink/Public/AsyncAction",
                "XGMaxKBLink/Public/Log",
                "XGMaxKBLink/Public/Type",

            }
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"HTTP"
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
                "Projects",
                "Json",
                "JsonUtilities",
            }
			);


        //Need to make sure Android has Launch module.
        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PrivateIncludePathModuleNames.AddRange(
                new string[] {
                    "Launch"
                }
            );
        }
    }
}
