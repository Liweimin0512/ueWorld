// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class unWorld : ModuleRules
{
	public unWorld(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine"
		});

		PrivateDependencyModuleNames.AddRange(new string[]{
			"VictoryBPLibrary",
			"ProceduralMeshComponent",
			"InputCore",
			"HeadMountedDisplay",
			"GameplayAbilities",
			"GameplayTags",
			"GameplayTasks"
			//"SimplexNoise"
		});
		PrivateIncludePaths.AddRange(new string[]
		{
			"SimplexNoise/Private"
		});
	}
}
