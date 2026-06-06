using UnrealBuildTool;

public class Characters: ModuleRules
{
    public Characters(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] {
            "Core", 
            "CoreUObject", 
            "Engine",
            "EnhancedInput"
        });
    }
}
