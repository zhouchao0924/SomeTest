#include "Register/SimpleRegister.h"
#include "Register/SimpleUEViewConfigurationInfor.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "Register/SimpleUEViewConfigurationInfor.h"
#include "Widgets/Input/SButton.h"
#include "SimpleUnrealPakViewType.h"
#include "SimpleUnrealPak.h"

#define LOCTEXT_NAMESPACE "SimpleRegister"

void SimpleRegister::Construct(const FArguments& InArgs, USimpleUEViewConfigurationInfor *Infor)
{
	ConfigInfor = Infor;

	auto& PropertyModule = FModuleManager::LoadModuleChecked< FPropertyEditorModule >("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	ConfigPanel = PropertyModule.CreateDetailView(DetailsViewArgs);

	ConfigPanel->SetObject(Infor);

	ChildSlot
	[
		SNew(SVerticalBox)

		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBox)
			.WidthOverride(300)
			.MinDesiredWidth(300)
			.MaxDesiredWidth(300)
			[
				SNew(SBorder)
				[
					ConfigPanel.ToSharedRef()
				]
			]
		]

		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SButton)
			.Text(LOCTEXT("AnalysisPak", "Analysis Pak"))
			.HAlign(HAlign_Center)
			.OnClicked(this, &SimpleRegister::OnClicked)
			.TextStyle(FEditorStyle::Get(), "ContentBrowser.TopBar.Font")
		]
	];
}

FReply SimpleRegister::OnClicked()
{
	SCOPE_PLATFORMFILE(SCopePlatformFile);

	TArray<FString> Filenames;
	//Filenames.Add(TEXT("G:/HotUpdate/Content/Geometry/Meshes/1M_Cube.uasset"));
	//Filenames.Add(TEXT("G:/HotUpdate/Content/Geometry/Meshes/1M_Cube_Chamfer.uasset"));
	//Filenames.Add(TEXT("G:/HotUpdate/Content/Mannequin/Character/Mesh/SK_Mannequin.uasset"));
	//Filenames.Add(TEXT("G:/HotUpdate/Content/SS/lambert1.uasset"));
	//Filenames.Add(TEXT("G:/HotUpdate/Content/StarterContent/Architecture/Floor_400x400.uasset"));
	//Filenames.Add(TEXT("G:/HotUpdate/Content/StarterContent/Audio/Collapse_Cue.uasset"));
	//Filenames.Add(TEXT("G:/HotUpdate/Content/StarterContent/Audio/Collapse01.uasset"));
	//Filenames.Add(TEXT("G:/HotUpdate/Content/StarterContent/Audio/Collapse_2.uasset"));

	FSimpleUnrealPakModule &SimpleUnrealPakModule = FModuleManager::LoadModuleChecked<FSimpleUnrealPakModule>(TEXT("SimpleUnrealPak"));
	
	if (!SCopePlatformFile.DoesPakExist(ConfigInfor->PakPath.FilePath))
	{
		FSimpleUnrealPakConfig Config;
		Config.AES = ConfigInfor->AES;
		SCopePlatformFile.InitConfig(Config);

		if (EMountInfor::MOUNT_SUCCESS == SimpleUnrealPakModule.MountPak(ConfigInfor->PakPath.FilePath, FSimpleAssetFileName::PakTest, INDEX_NONE, ConfigInfor->MountPoint))
		{
			SCopePlatformFile.GetFilenamesInPakFile(ConfigInfor->PakPath.FilePath, Filenames);
			if (Filenames.Num())
			{
				SimpleOneParamDelegate.ExecuteIfBound(Filenames);
				LastPaths = ConfigInfor->PakPath.FilePath;
			}
			else
			{
				SimpleUnrealPakModule.UnmountPak(ConfigInfor->PakPath.FilePath);
			}		
		}
	}
	else
	{
		if (SimpleUnrealPakModule.FindCacheAsset(ConfigInfor->PakPath.FilePath, Filenames))
		{
			SimpleOneParamDelegate.ExecuteIfBound(Filenames);
			LastPaths = ConfigInfor->PakPath.FilePath;
		}	
	}
	
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE