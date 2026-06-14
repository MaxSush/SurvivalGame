using UnrealBuildTool;

public class CoreData: ModuleRules
{
    public CoreData(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] {
            "Core", 
            "CoreUObject", 
            "Engine"
        });
    }
}
