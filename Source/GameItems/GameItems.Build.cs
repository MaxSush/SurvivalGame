using UnrealBuildTool;

public class GameItems: ModuleRules
{
    public GameItems(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine"});
    }
}
