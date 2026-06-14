using UnrealBuildTool;

public class ComponentInterfaces: ModuleRules
{
    public ComponentInterfaces(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "GameItems"
        });
    }
}
