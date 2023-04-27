// Third Player Shooter, All Rights Reserved

using UnrealBuildTool;

public class TPShooter : ModuleRules
{
    public TPShooter(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core", 
            "CoreUObject", 
            "Engine", 
            "InputCore",
            "Niagara",
            "PhysicsCore",
            "AIModule",
            "GameplayTasks",
            "NavigationSystem"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        PublicIncludePaths.AddRange(new string[]
        {
            "TPShooter/Public/Player", 
            "TPShooter/Public/Components", 
            "TPShooter/Public/Dev",
            "TPShooter/Public/Weapon",
            "TPShooter/Public/UI",
            "TPShooter/Public/Animations",
            "TPShooter/Public/Pickup",
            "TPShooter/Public/Weapon/Components",
            "TPShooter/Public/AI",
            "TPShooter/Public/AI/Tasks",
            "TPShooter/Public/AI/Services",
            "TPShooter/Public/AI/EQS",
            "TPShooter/Public/AI/Decorators"
        });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
