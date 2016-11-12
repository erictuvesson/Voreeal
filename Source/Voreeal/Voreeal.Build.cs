using UnrealBuildTool;
using System.IO;

public class Voreeal : ModuleRules
{
    private string PolyVoxPath
    {
        get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../ThirdParty/PolyVox/")); }
    }

    public Voreeal(TargetInfo Target)
    {
        MinFilesUsingPrecompiledHeaderOverride = 1;
        bFasterWithoutUnity = true;

        PublicDependencyModuleNames.AddRange(
            new string[] 
            {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "RHI",
                "RenderCore",
                "ShaderCore",
                "ProceduralMeshComponent"
            });

        /// 
        /// Register PolyVox
        /// 

        PublicIncludePaths.Add(Path.Combine(PolyVoxPath, "include"));
        Definitions.Add(string.Format("WITH_POLYVOX_BINDING={0}", 1));
    }
}
