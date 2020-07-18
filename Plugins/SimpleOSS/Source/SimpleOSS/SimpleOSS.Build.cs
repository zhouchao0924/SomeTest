// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class SimpleOSS : ModuleRules
{
    private string OSSThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../OSSThirdParty/")); }
    }

    public SimpleOSS(ReadOnlyTargetRules Target) : base(Target)
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
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

        string OssIncludePath = Path.Combine(OSSThirdPartyPath, "include");
        PublicIncludePaths.Add(OssIncludePath);
        string AlibabacloudPath = Path.Combine(OssIncludePath, "alibabacloud/oss");
        PublicIncludePaths.Add(AlibabacloudPath);
        PublicIncludePaths.Add(Path.Combine(AlibabacloudPath, "auth"));
        PublicIncludePaths.Add(Path.Combine(AlibabacloudPath, "client"));
        PublicIncludePaths.Add(Path.Combine(AlibabacloudPath, "http"));
        PublicIncludePaths.Add(Path.Combine(AlibabacloudPath, "model"));
        PublicIncludePaths.Add(Path.Combine(AlibabacloudPath, "utils"));

        PublicIncludePaths.Add(Path.Combine(OssIncludePath, "curl"));
        PublicIncludePaths.Add(Path.Combine(OssIncludePath, "openssl"));

        string LibPath = Path.Combine(OSSThirdPartyPath, "lib");
        PublicLibraryPaths.Add(LibPath);
        /*PublicAdditionalLibraries*/
        PublicAdditionalLibraries.Add(Path.Combine(OSSThirdPartyPath, "lib", "alibabacloud-oss-cpp-sdk.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(LibPath, "x64", "libcurl.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(LibPath, "x64", "libeay32.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(LibPath, "x64", "ssleay32.lib"));

        PublicDelayLoadDLLs.Add(Path.Combine(LibPath, "x64", "libcurl.dll"));
        PublicDelayLoadDLLs.Add(Path.Combine(LibPath, "x64", "libeay32.dll"));
        PublicDelayLoadDLLs.Add(Path.Combine(LibPath, "x64", "ssleay32.dll"));
        PublicDelayLoadDLLs.Add(Path.Combine(LibPath, "x64", "zlibwapi.dll"));
    }
}
