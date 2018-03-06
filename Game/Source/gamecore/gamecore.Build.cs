// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class gamecore : ModuleRules
{
	public gamecore( ReadOnlyTargetRules Target ) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange( new string[]{"Core", "CoreUObject", "Engine", "InputCore", "AiModule", "AnimGraphRuntime", "GameplayTasks" } );

		PrivateDependencyModuleNames.AddRange( new string[]{} );
	}
}
