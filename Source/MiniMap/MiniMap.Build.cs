// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MiniMap : ModuleRules
{
	public MiniMap(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "GameplayTags", "GameplayTasks", "GameplayAbilities", "ModularGameplay" });

        PublicIncludePaths.AddRange(new string[] {"MiniMap"});
    }
}
