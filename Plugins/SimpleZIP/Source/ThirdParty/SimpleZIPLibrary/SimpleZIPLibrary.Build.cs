// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class SimpleZIPLibrary : ModuleRules
{
    public string ZIPProjectDirectory
    {
        get
        {
            return Path.GetFullPath(
                  Path.Combine(ModuleDirectory, "../../../../../")
            );
        }
    }

    private void CopyZIPDllAndLibToProjectBinaries(string Filepath, ReadOnlyTargetRules Target)
    {
        string BinariesDirectory = Path.Combine(ZIPProjectDirectory, "Binaries", Target.Platform.ToString());

        string Filename = Path.GetFileName(Filepath);

        if (!Directory.Exists(BinariesDirectory))
        {
            Directory.CreateDirectory(BinariesDirectory);
        }

        File.Copy(Filepath, Path.Combine(BinariesDirectory, Filename), true);

        RuntimeDependencies.Add(Path.Combine(BinariesDirectory, Filename));
    }

    public SimpleZIPLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// Add the import library
			PublicLibraryPaths.Add(Path.Combine(ModuleDirectory, "x64", "Release"));
			PublicAdditionalLibraries.Add("ZIP_SDK_Library.lib");

			// Delay-load the DLL, so we can load it from the right place first
			PublicDelayLoadDLLs.Add("ZIP_SDK_Library.dll");

            CopyZIPDllAndLibToProjectBinaries(Path.Combine(ModuleDirectory,"x64", "Release", "ZIP_SDK_Library.dll"),Target);
            CopyZIPDllAndLibToProjectBinaries(Path.Combine(ModuleDirectory,"x64", "Release", "ZIP_SDK_Library.lib"), Target);
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
          //  PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "libExampleLibrary.dylib"));
        }
    }
}
