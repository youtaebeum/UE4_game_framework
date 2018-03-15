using UnrealBuildTool;

namespace UnrealBuildTool.Rules {

public class UFSMEditor : ModuleRules {
    public UFSMEditor(ReadOnlyTargetRules Target) : base(Target) {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnforceIWYU = true;
		//
        PublicDependencyModuleNames.AddRange(
            new string[] {
                "UFSM",
                "Core",
                "Engine",
                "CoreUObject",
                "UFSMDeveloper"
            }
        );
        //
        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "Slate",
                "Projects",
                "UnrealEd",
                "SlateCore",
                "AssetTools",
                "EditorStyle",
                "LevelEditor",
				"GraphEditor",
                "PropertyEditor",
				"BlueprintGraph"
            }
        );
    }
}

}