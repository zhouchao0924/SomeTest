// Some copyright should be here...
using System.IO;
using UnrealBuildTool;

public class SimpleVersionControl : ModuleRules
{
    public string ProjectDirectory
    {
        get
        {
            return Path.GetFullPath(
                  Path.Combine(ModuleDirectory, "../../../../")
            );
        }
    }

    private void CopyContentProjectBinaries(string Filepath, ReadOnlyTargetRules Target)
    {
        string BinariesDirectory = Path.Combine(ProjectDirectory, "Binaries", Target.Platform.ToString());

        string Filename = Path.GetFileName(Filepath);

        if (!Directory.Exists(BinariesDirectory))
        {
            Directory.CreateDirectory(BinariesDirectory);
        }

        File.Copy(Filepath, Path.Combine(BinariesDirectory, Filename), true);

        RuntimeDependencies.Add(Path.Combine(BinariesDirectory, Filename));
    }

    public SimpleVersionControl(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
                "Json",
                "SimpleHTTP"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

        string ResetGamePath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../"));
        CopyContentProjectBinaries(Path.Combine(ResetGamePath, "Content", Target.Platform.ToString(), "ResetGame.exe"), Target);
    }
}
