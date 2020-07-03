#include "MeshImportPanel.h"
#include "Widgets/Input/SButton.h"
#include "AssetToolsModule.h"
#include "DetailWidgetRow.h"
#include "EditorDirectories.h"
#include "KismetProceduralMeshLibrary.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Engine/StaticMesh.h"

#define LOCTEXT_NAMESPACE "FMeshlImportDetail"

ASSETSLATEUI_API TWeakObjectPtr<UCustomAsset> CustomAssetInstance = nullptr;

void FMeshlImportDetail::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	HeaderRow
	.NameContent()
	[
		SAssignNew(EditableTextBox,SEditableTextBox)
		.OnTextCommitted(this, &FMeshlImportDetail::OnLabelsRootTextCommited)
	]
	.ValueContent()
	.MinDesiredWidth(125.0f)
	.MaxDesiredWidth(325.0f)
	[
		SNew(SButton)
		.OnClicked(this, &FMeshlImportDetail::HandleImportClicked)
		.Text(LOCTEXT("Import", "Import Mesh"))
	];
}

void FMeshlImportDetail::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	HandleMeshSavePaths = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FMeshlImport, MeshSavePaths));

	//还原路径
	if (EditableTextBox.IsValid())
	{
		FText EventValueStr;
		HandleMeshSavePaths->GetValue(EventValueStr);

		if (HandleMeshSavePaths.IsValid())
		{
			EditableTextBox->SetText(EventValueStr);
		}
	}
}

TSharedRef<IPropertyTypeCustomization> FMeshlImportDetail::MakeInstance()
{
	return MakeShareable(new FMeshlImportDetail);
}

FReply FMeshlImportDetail::HandleImportClicked()
{
	FString MeshNewPath = TEXT("/Game");

	//获取路径值
	FText EventValueStr;
	HandleMeshSavePaths->GetValue(EventValueStr);

	FString LastPaths;
	if (EventValueStr.IsEmpty())
	{
		MeshNewPath = TEXT("/Game");

		//指定资源
		FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
		AssetToolsModule.Get().ImportAssetsWithDialog(MeshNewPath);

		//适用于任何模型路径导入
		FString InLastPaths = FEditorDirectories::Get().GetLastDirectory(ELastDirectory::GENERIC_IMPORT);

		////F:/PJ/PluginCore/Content/well1.obj
		FString L, R, S, K;
		InLastPaths.Split("/", &L, &R, ESearchCase::IgnoreCase, ESearchDir::FromEnd);//L = F:/PJ/PluginCore/Content/  R = well1.obj
		R.Split(".", &S, &K);//s = well1  k = obj
		MeshNewPath.Append("/" + S);//Game/well1
	}
	else
	{
		MeshNewPath += (TEXT("/") + EventValueStr.ToString());
	}

	if (CustomAssetInstance.IsValid())
	{
		//FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
		//AssetToolsModule.Get().ImportAssetsWithDialog(MeshNewPath);

		////只能将模型导入到Content
		////F:/PJ/PluginCore/Content/well1.obj
		//FString LastPaths = FEditorDirectories::Get().GetLastDirectory(ELastDirectory::GENERIC_IMPORT);
		//FString L, R, S, K;
		//LastPaths.Split(".", &L, &R);//L = F:/PJ/PluginCore/Content/well1  R = obj
		//L.Split("PluginCore", &S, &K);// K  = /Content/well1 
		//K.ReplaceInline(TEXT("Content"), TEXT("Game"));// K = /Game/well1

		UStaticMesh *ObjectMesh = Cast<UStaticMesh>(StaticLoadObject(UObject::StaticClass(), NULL, *MeshNewPath));
		if (ObjectMesh)
		{
			for (int32 LOD = 0; LOD < ObjectMesh->RenderData->LODResources.Num(); LOD++)
			{
				FCustomMeshLOD CustomMeshLOD;
				for (int32 Section = 0; Section < ObjectMesh->RenderData->LODResources[LOD].Sections.Num(); Section++)
				{
					FCustomMeshSection CustomMeshSection;
					UKismetProceduralMeshLibrary::GetSectionFromStaticMesh(
						ObjectMesh,
						LOD,
						Section,
						CustomMeshSection.Vertices,
						CustomMeshSection.Triangles, 
						CustomMeshSection.Normals,
						CustomMeshSection.UV0,
						CustomMeshSection.Tangents
					);

					CustomMeshLOD.MeshLOD.Add(CustomMeshSection);
				}

				CustomAssetInstance->MeshData.Add(CustomMeshLOD);
			}
		}
	}

	return FReply::Handled();
}

void FMeshlImportDetail::OnLabelsRootTextCommited(const FText& InText, ETextCommit::Type InCommitType) const
{
	if (HandleMeshSavePaths.IsValid())
	{
		HandleMeshSavePaths->SetValue(InText);
	}
}

#undef  LOCTEXT_NAMESPACE
