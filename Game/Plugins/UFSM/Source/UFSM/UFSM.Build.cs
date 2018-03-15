using UnrealBuildTool;
using System.IO;

namespace UnrealBuildTool.Rules {

public class UFSM : ModuleRules {
    public UFSM(ReadOnlyTargetRules Target) : base(Target) {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnforceIWYU = true;
		//
		PublicDependencyModuleNames.AddRange(new string[] {"Core","Engine","CoreUObject","AIModule","GameplayTasks"});
		PrivateIncludePaths.Add(Path.Combine(ModuleDirectory,"Private"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory,"Public"));
	}
}

}