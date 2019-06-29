// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class StoneAgeColony : ModuleRules
{
	public StoneAgeColony(ReadOnlyTargetRules Target) : base(Target)
	{
        // Test for fastening compile times
        //MinFilesUsingPrecompiledHeaderOverride = 1;
        //bFasterWithoutUnity = true;



        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "UMG", "InputCore", "HeadMountedDisplay", "AIModule", "GameplayTasks", "NavigationSystem" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        
    }
}
