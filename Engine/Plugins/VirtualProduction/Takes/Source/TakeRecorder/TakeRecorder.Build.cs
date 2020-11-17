// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TakeRecorder : ModuleRules
{
	public TakeRecorder(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"AppFramework",
				"AssetRegistry",
				"AssetTools",
				"ContentBrowser",
				"Core",
				"CoreUObject",
				"EditorStyle",
				"EditorWidgets",
				"Engine",
				"InputCore",
				"LevelEditor",
				"LevelSequence",
				"LevelSequenceEditor",
				"MovieScene",
				"MovieSceneTools",
				"PropertyEditor",
				"TakesCore",
				"TimeManagement",
				"Settings",
				"Slate",
				"SlateCore",
				"UMG",
				"UnrealEd",
				"WorkspaceMenuStructure",
				"Analytics",
				"ToolMenus",
			}
		);

		PublicDependencyModuleNames.AddRange(
			new string[] {
				"UMG",
                "TakeTrackRecorders",
                "SerializedRecorderInterface",
                "Sequencer",
            }
        );

		PrivateIncludePaths.AddRange(
			new string[] {
				"TakeRecorder/Private",
				"TakeRecorder/Public",
                "TakeRecorderSources/Private",
            }
        );
    }
}
