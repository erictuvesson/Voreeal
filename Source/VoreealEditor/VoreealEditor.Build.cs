using UnrealBuildTool;

public class VoreealEditor : ModuleRules
{
	public VoreealEditor(TargetInfo Target)
	{
		PrivateIncludePaths.AddRange(
			new string[]
			{
				"VoreealEditor/Private"
			});

		PrivateDependencyModuleNames.AddRange(
			new string[] 
			{
				"Core",
				"CoreUObject",
				"Slate",
				"SlateCore",
				"Engine",
				"InputCore",
				"UnrealEd", // for FAssetEditorManager
				"KismetWidgets",
				"Kismet",  // for FWorkflowCentricApplication
				"PropertyEditor",
				"RenderCore",
				"ContentBrowser",
				"WorkspaceMenuStructure",
				"EditorStyle",
				"EditorWidgets",
				"Projects",

				"Voreeal"
			});

		PrivateIncludePathModuleNames.AddRange(
			new string[] 
			{
				"Settings",
				"IntroTutorials",
				"AssetTools"
			});

		DynamicallyLoadedModuleNames.AddRange(
			new string[] 
			{
				"AssetTools"
			});
	}
}