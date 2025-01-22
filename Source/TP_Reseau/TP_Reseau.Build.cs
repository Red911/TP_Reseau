// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TP_Reseau : ModuleRules
{
	public TP_Reseau(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "Niagara", "OnlineSubsystem", "OnlineSubsystemUtils" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });
	}
}
