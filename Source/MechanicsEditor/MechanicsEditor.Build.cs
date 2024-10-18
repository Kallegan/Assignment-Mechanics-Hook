// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MechanicsEditor : ModuleRules
{
	public MechanicsEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"UnrealEd", // Unreal Editor module
			"Slate", // Slate UI framework, if needed for editor tools
			"SlateCore" // Core part of the Slate UI framework
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"InputCore", // Handles input processing in the editor
			"Mechanics", // If this module depends on the Mechanics module
			"EditorStyle" // Optional: Add if you want to style your custom tools
		});
	}
}
