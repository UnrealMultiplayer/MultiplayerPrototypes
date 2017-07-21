// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Parkour : ModuleRules
{
	public Parkour(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "OnlineSubsystem", "UMG" });
        //DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam"); // Need to add to ensure steam loads on other machines.
    }
}
