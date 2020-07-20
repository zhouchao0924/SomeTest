// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
/*
 下面的网址讲解了如何使用本套插件
 DocURL：			https://zhuanlan.zhihu.com/p/83673743
 
 下面的网址讲解如何开发当前的插件
 MarketplaceURL :  https://www.aboutcg.org/courseDetails/682/introduce
 
 如果想了解更多关于UE4教程请参考：
 URL :				https://zhuanlan.zhihu.com/p/60117613
 
 如果想系统了解人宅系列教程以及相关插件迭代更新 可以关注我的博客
 URL :				http://renzhai.net/
 
 如果想了解我们下一节的课程安排可以 可以在微信公众号搜所 人宅 关注即可 我们会推送很多富有技术的文章
 
 新浪博客 https://weibo.com/BZRZ/profile?s=6cm7D0  //这个博客从16年到现在 有三四年没怎么用过 以后说不定可以重新启用 先发个连接
 
 */
using System.IO;
using UnrealBuildTool;

public class SimpleOSS : ModuleRules
{
    private string ThirdPartyPath
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
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);


            //string ThirdPartyPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../ThirdParty/"));
            string OssPath = Path.Combine(ThirdPartyPath, "include");
            PublicIncludePaths.Add(OssPath);
            string AlibabacloudPath = Path.Combine(OssPath, "alibabacloud/oss");
            PublicIncludePaths.Add(AlibabacloudPath);
            PublicIncludePaths.Add(Path.Combine(AlibabacloudPath, "auth"));
            PublicIncludePaths.Add(Path.Combine(AlibabacloudPath, "client"));
            PublicIncludePaths.Add(Path.Combine(AlibabacloudPath, "http"));
            PublicIncludePaths.Add(Path.Combine(AlibabacloudPath, "model"));
            PublicIncludePaths.Add(Path.Combine(AlibabacloudPath, "utils"));

            PublicIncludePaths.Add(Path.Combine(OssPath, "curl"));
            PublicIncludePaths.Add(Path.Combine(OssPath, "openssl"));

            string LibPath = Path.Combine(ThirdPartyPath, "lib");
            PublicLibraryPaths.Add(LibPath);
            PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "lib", "alibabacloud-oss-cpp-sdk.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(LibPath, "X64", "libcurl.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(LibPath, "X64", "libeay32.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(LibPath, "X64", "ssleay32.lib"));

            PublicDelayLoadDLLs.Add(Path.Combine(LibPath, "x64", "libcurl.dll"));
            PublicDelayLoadDLLs.Add(Path.Combine(LibPath, "x64", "libeay32.dll"));
            PublicDelayLoadDLLs.Add(Path.Combine(LibPath, "x64", "ssleay32.dll"));
            PublicDelayLoadDLLs.Add(Path.Combine(LibPath, "x64", "zlibwapi.dll"));
    }
}
