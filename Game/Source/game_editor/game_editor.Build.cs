// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class game_editor : ModuleRules
{
	public game_editor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "UMG",
            "AIModule",
            "GameplayTasks",
            "gamecore"});
        
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "EditorStyle" });
    }
}
