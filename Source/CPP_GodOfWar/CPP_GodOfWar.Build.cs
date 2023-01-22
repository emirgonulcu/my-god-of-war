// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CPP_GodOfWar : ModuleRules
{
	public CPP_GodOfWar(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "AIModule", "NavigationSystem", "UMG", "Slate", "SlateCore", "Niagara"});
	}
}
