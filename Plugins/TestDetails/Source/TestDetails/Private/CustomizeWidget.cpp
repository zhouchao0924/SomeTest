#include "CustomizeWidget.h"
#include "PropertyEditing.h"
#include "CustomizeObject.h"

void SCustomizeWidget::Construct(const FArguments& InArgs)
{
	CustomizeObject = InArgs._CustomizeObject;

	auto& PropertyModule = FModuleManager::LoadModuleChecked< FPropertyEditorModule >("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs(false, false, true, FDetailsViewArgs::HideNameArea, true);
	ConfigPanel = PropertyModule.CreateDetailView(DetailsViewArgs);

//	ConfigPanel->SetObject(UCustomizeObject::StaticClass()->GetDefaultObject(true), true);
	ConfigPanel->SetObject(CustomizeObject.Get());

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
		];
}

