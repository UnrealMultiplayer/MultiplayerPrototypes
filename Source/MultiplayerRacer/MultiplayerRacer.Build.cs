// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MultiplayerRacer : ModuleRules
{
	public MultiplayerRacer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "PhysXVehicles", "HeadMountedDisplay" });

        DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");

        Definitions.Add("HMD_MODULE_INCLUDED=1");
	}
}
