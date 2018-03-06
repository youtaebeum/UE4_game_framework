// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class gamecore_editor : ModuleRules
{
	public gamecore_editor( ReadOnlyTargetRules Target ) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange( new string[]{"Core", "CoreUObject", "Engine", "InputCore", "NavMesh", "gamecore" } );

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Slate",
				"SlateCore",
				"InputCore",
				"UnrealEd",
				"AssetRegistry",
				//"Json",
				//"CollectionManager",
				"ContentBrowser",
				"WorkspaceMenuStructure",
				"EditorStyle",
				"AssetTools",
				"PropertyEditor",
				"GraphEditor",
				"AnimGraph",
				"AnimGraphRuntime",
				"BlueprintGraph",
                //"ReferenceViewer",
                //"SandboxFile",
			}
		);
    }
}
