#include "Slate/SVersionControlWdiget.h"
#include "Field/VersionControlInfo.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "Widgets/Input/SButton.h"
#include "Settings/SimpleUnrealPakSettings.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Settings/SimpleVCSettings.h"
#include "SimpleVersionControlEditor.h"

#define LOCTEXT_NAMESPACE "VersionControlWdiget"

void SVersionControlWdiget::Construct(const FArguments& InArgs, UVersionControlInfo *Infor, USimpleUnrealPakSettings *InPakSettings, USimpleVCSettings *InVCSettings)
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked< FPropertyEditorModule >("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;

	ConfigPanel = PropertyModule.CreateDetailView(DetailsViewArgs);
	ConfigPanel->SetObject(Infor);

	ConfigPanelSettings = PropertyModule.CreateDetailView(DetailsViewArgs);
	ConfigPanelSettings->SetObject(InPakSettings);

	PakPanelSettings = PropertyModule.CreateDetailView(DetailsViewArgs);
	PakPanelSettings->SetObject(InVCSettings);

	ChildSlot
	[
		SNew(SScrollBox)
		.Orientation(Orient_Vertical)

		+ SScrollBox::Slot()
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
						ConfigPanelSettings.ToSharedRef()
					]
				]
			]

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
						PakPanelSettings.ToSharedRef()
					]
				]
			]

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
			.Padding(10.f)
			.AutoHeight()
			[
				SNew(SButton)
				.Text(LOCTEXT("UpdateMainVersion", "Update Main Version"))
				.HAlign(HAlign_Center)
				.OnClicked(this, &SVersionControlWdiget::OnUpdateMainVersion)
				.TextStyle(FEditorStyle::Get(), "ContentBrowser.TopBar.Font")
			]
		]
	];
}

FReply SVersionControlWdiget::OnUpdateMainVersion()
{
	FSimpleVersionControlEditorModule * VersionControl = FModuleManager::GetModulePtr<FSimpleVersionControlEditorModule>(TEXT("SimpleVersionControlEditor"));
	if (VersionControl)
	{
		VersionControl->RegisteringMainVersionCallbacks();
	}
	
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE