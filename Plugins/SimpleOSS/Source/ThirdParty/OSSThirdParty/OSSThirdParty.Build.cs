// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class OSSThirdParty : ModuleRules
{
    public OSSThirdParty(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.External;

        string ThirdPartyPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../OSSThirdParty/"));
        string OssPathInclude = Path.Combine(ThirdPartyPath, "oss_c_sdk");
        string Third_partyPath = Path.Combine(ThirdPartyPath, "third_party/include");
        PublicIncludePaths.Add(OssPathInclude);
        PublicIncludePaths.Add(Third_partyPath);

        //阿里云SDK依赖的第三方库的include
        {
            PublicIncludePaths.Add(Path.Combine(Third_partyPath, "curl"));
            PublicIncludePaths.Add(Path.Combine(Third_partyPath, "mxml"));
            PublicIncludePaths.Add(Path.Combine(Third_partyPath, "aprutil"));
            PublicIncludePaths.Add(Path.Combine(Third_partyPath, "apr"));
        }

        string OSSPlatformFile = "";
        if (Target.Platform == UnrealTargetPlatform.Win64)
		{
            // Add the import library
            //	PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "Debug", "alibabacloud-oss-cpp-sdk.lib"));

            // Delay-load the DLL, so we can load it from the right place first
            //PublicDelayLoadDLLs.Add("ExampleLibrary.dll");

            // Ensure that the DLL is staged along with the executable
   //         RuntimeDependencies.Add("$(PluginDir)/SimpleOSS/Source/ThirdParty/OSSThirdParty/x64/Release/oss_c_sdk.lib");
            //RuntimeDependencies.Add("$(PluginDir)/SimpleOSS/Source/ThirdParty/OSSThirdParty/third_party/lib/x64/libapr-1.dll");
            //RuntimeDependencies.Add("$(PluginDir)/SimpleOSS/Source/ThirdParty/OSSThirdParty/third_party/lib/x64/libapriconv-1.dll");
            //RuntimeDependencies.Add("$(PluginDir)/SimpleOSS/Source/ThirdParty/OSSThirdParty/third_party/lib/x64/libaprutil-1.dll");
            //RuntimeDependencies.Add("$(PluginDir)/SimpleOSS/Source/ThirdParty/OSSThirdParty/third_party/lib/x64/libcurl.dll");
            //RuntimeDependencies.Add("$(PluginDir)/SimpleOSS/Source/ThirdParty/OSSThirdParty/third_party/lib/x64/libexpat.dll");
            //RuntimeDependencies.Add("$(PluginDir)/SimpleOSS/Source/ThirdParty/OSSThirdParty/third_party/lib/x64/mxml1.dll");
            //RuntimeDependencies.Add("$(PluginDir)/SimpleOSS/Source/ThirdParty/OSSThirdParty/third_party/lib/x64/libeay32.dll");
            //RuntimeDependencies.Add("$(PluginDir)/SimpleOSS/Source/ThirdParty/OSSThirdParty/third_party/lib/x64/ssleay32.dll");
            //RuntimeDependencies.Add("$(PluginDir)/SimpleOSS/Source/ThirdParty/OSSThirdParty/third_party/lib/x64/zlibwapi.dll");

            //RuntimeDependencies.Add("$(PluginDir)/SimpleOSS/Source/ThirdParty/OSSThirdParty/third_party/lib/x64/libapr-1.lib");
            //RuntimeDependencies.Add("$(PluginDir)/SimpleOSS/Source/ThirdParty/OSSThirdParty/third_party/lib/x64/libapriconv-1.lib");
            //RuntimeDependencies.Add("$(PluginDir)/SimpleOSS/Source/ThirdParty/OSSThirdParty/third_party/lib/x64/libaprutil-1.lib");
            //RuntimeDependencies.Add("$(PluginDir)/SimpleOSS/Source/ThirdParty/OSSThirdParty/third_party/lib/x64/libcurl.lib");
            //RuntimeDependencies.Add("$(PluginDir)/SimpleOSS/Source/ThirdParty/OSSThirdParty/third_party/lib/x64/libexpat.lib");
            //RuntimeDependencies.Add("$(PluginDir)/SimpleOSS/Source/ThirdParty/OSSThirdParty/third_party/lib/x64/mxml1.lib");

            OSSPlatformFile = Path.Combine(ThirdPartyPath, "third_party/lib/x64");
            PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "x64/Release", "oss_c_sdk.lib"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Win32)
        {
            OSSPlatformFile = Path.Combine(ThirdPartyPath, "third_party/lib/Win32");
            PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "Win32/Release", "oss_c_sdk.lib"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            //PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "libExampleLibrary.dylib"));
            //RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/TLibrary/Mac/Release/libExampleLibrary.dylib");
        }

        PublicAdditionalLibraries.Add(Path.Combine(OSSPlatformFile, "libapr-1.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(OSSPlatformFile, "libapriconv-1.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(OSSPlatformFile, "libaprutil-1.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(OSSPlatformFile, "libcurl.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(OSSPlatformFile, "libexpat.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(OSSPlatformFile, "mxml1.lib"));

        PublicDelayLoadDLLs.Add(Path.Combine(OSSPlatformFile, "libapr-1.dll"));
        PublicDelayLoadDLLs.Add(Path.Combine(OSSPlatformFile, "libapriconv-1.dll"));
        PublicDelayLoadDLLs.Add(Path.Combine(OSSPlatformFile, "libaprutil-1.dll"));
        PublicDelayLoadDLLs.Add(Path.Combine(OSSPlatformFile, "libcurl.dll"));
        PublicDelayLoadDLLs.Add(Path.Combine(OSSPlatformFile, "libexpat.dll"));
        PublicDelayLoadDLLs.Add(Path.Combine(OSSPlatformFile, "mxml1.dll"));
        PublicDelayLoadDLLs.Add(Path.Combine(OSSPlatformFile, "libeay32.dll"));
        PublicDelayLoadDLLs.Add(Path.Combine(OSSPlatformFile, "ssleay32.dll"));
        PublicDelayLoadDLLs.Add(Path.Combine(OSSPlatformFile, "zlibwapi.dll"));

        bUsePrecompiled = true;
    }
}
