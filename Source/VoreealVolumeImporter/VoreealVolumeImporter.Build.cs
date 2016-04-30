using UnrealBuildTool;
using System.IO;

public class VoreealVolumeImporter : ModuleRules
{
    private string ModulePath
    {
        get { return Path.GetDirectoryName(RulesCompiler.GetModuleFilename(this.GetType().Name)); }
    }

    private string PolyVoxPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../PolyVox/")); }
    }

    public VoreealVolumeImporter(TargetInfo Target)
    {
        PublicIncludePaths.Add(Path.Combine(PolyVoxPath, "include"));

        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "CoreUObject",
                "Engine",
                "UnrealEd",
                
                "Voreeal",
                "VoreealEditor"
            });

        PrivateIncludePathModuleNames.AddRange(
            new string[] {
                "AssetTools",
                "AssetRegistry"
            });

        DynamicallyLoadedModuleNames.AddRange(
            new string[] {
                "AssetTools",
                "AssetRegistry"
            });
    }
}