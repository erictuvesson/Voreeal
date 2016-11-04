using UnrealBuildTool;

public class VoreealEditor : ModuleRules
{
	public VoreealEditor(TargetInfo Target)
	{
		//PrivateIncludePaths.Add("PolyVoxEditor/Private");

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Json",
				"Slate",
				"SlateCore",
				"Engine",
				"InputCore",
				"UnrealEd", // for FAssetEditorManager
				"KismetWidgets",
				"Kismet",  // for FWorkflowCentricApplication
				"PropertyEditor",
				"RenderCore",
				"Paper2D",
				"ContentBrowser",
				"WorkspaceMenuStructure",
				"EditorStyle",
				"EditorWidgets",
				"Projects",

				"Voreeal"
			});

		PrivateIncludePathModuleNames.AddRange(
			new string[] {
				"Settings",
				"IntroTutorials",
				"AssetTools"
			});

		DynamicallyLoadedModuleNames.AddRange(
			new string[] {
				"AssetTools"
			});
	}
}