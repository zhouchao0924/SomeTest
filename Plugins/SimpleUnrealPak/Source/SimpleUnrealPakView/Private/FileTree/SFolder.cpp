#include "FileTree/SFolder.h"
#include "FileTree/SFile.h"
#include "Widgets/Layout/SExpandableArea.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SSpacer.h"
#include "EditorStyleSet.h"

#define LOCTEXT_NAMESPACE "Folder"

void SFolder::Construct(const FArguments& InArgs, const SimpleUnrealPakView::FFileList &FileList)
{
	TSharedPtr<SVerticalBox> FileArray = SNew(SVerticalBox);

	for (const auto &Tmp : FileList.Children)
	{
		if (Tmp.GetFileType() == SimpleUnrealPakView::EFileType::FOLDER)
		{
			FileArray->AddSlot()
			.AutoHeight()
			[
				SNew(SFolder, Tmp)
			];
		}
		else
		{
			FileArray->AddSlot()
			.AutoHeight()
			[
				SNew(SFile, Tmp)
			];
		}
	}

	ChildSlot
	[
		SAssignNew(Area, SExpandableArea)
		.BorderBackgroundColor(FLinearColor::Transparent)
		.InitiallyCollapsed(false)
		.Padding(FMargin(0.0f, 1.0f, 0.0f, 8.0f))
		.HeaderContent()
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SImage)
				.Image(this, &SFolder::GetFileTypeICO)
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(STextBlock)
				.Text(FText::Format(LOCTEXT("AnalyzingFilename", "{0}"), FText::FromString(FileList.Filename)))
			]
		]
		.BodyContent()
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SSpacer)
				.Size(FVector2D(20.f, 1.f))
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				FileArray.ToSharedRef()
			]
		]
	];
}

const FSlateBrush* SFolder::GetFileTypeICO() const
{
	return Area->IsExpanded()? 
		FEditorStyle::Get().GetBrush(TEXT("SceneOutliner.FolderClosed")):
		FEditorStyle::Get().GetBrush(TEXT("SceneOutliner.FolderOpen"));
}

#undef LOCTEXT_NAMESPACE