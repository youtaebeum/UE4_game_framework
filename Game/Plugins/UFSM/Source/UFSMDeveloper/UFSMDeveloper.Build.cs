using UnrealBuildTool;

namespace UnrealBuildTool.Rules {

public class UFSMDeveloper : ModuleRules {
    public UFSMDeveloper(ReadOnlyTargetRules Target) : base(Target) {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnforceIWYU = true;
		//
        PublicDependencyModuleNames.AddRange(
            new string[] {
                "UFSM",
                "Core",
                "Engine",
				"AnimGraph",
                "CoreUObject"
            }
        );
        //
        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "Projects",
                "SlateCore",
				"BlueprintGraph"
            }
        );
    }
}

}