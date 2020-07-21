#include "FileTree/SFile.h"
#include "Styling/CoreStyle.h"
#include "EditorStyleSet.h"
#include "Widgets/Input/SButton.h"
#include "SimpleUnrealPak.h"
#include "Engine/StaticMesh.h"
#include "SimpleUnrealPakViewType.h"
#include "KismetProceduralMeshLibrary.h"
#include "ProceduralMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "Engine/World.h"

#define LOCTEXT_NAMESPACE "File"

void SFile::Construct(const FArguments& InArgs, const SimpleUnrealPakView::FFileList &FileList)
{
	AssetPaths = FileList.Filename;
	{
		AssetPaths.RemoveFromEnd(FPaths::GetExtension(FileList.Filename, true));
	}
	// /Game/SS/A.usset
	FString AssetName = FPackageName::GetShortName(FileList.Filename);
	ChildSlot
		[
			SNew(SButton)
			.Text(FText::Format(LOCTEXT("AnalyzingFilename", "{0}"), FText::FromString(AssetName)))
			.HAlign(HAlign_Fill)
			.OnClicked(this, &SFile::OnClicked)
			.ButtonStyle(FCoreStyle::Get(), "NoBorder")
			.TextStyle(FEditorStyle::Get(), "FlatButton.DefaultTextStyle")
		];
}

FReply SFile::OnClicked()
{
	if (!AssetPaths.IsEmpty())
	{
		if (GMeshComponent && GProceduralMeshComponent)
		{
			for (AActor *Tmp: Actors)
			{
				Tmp->Destroy();
			}

			if (GProceduralMeshComponent)
			{
				GProceduralMeshComponent->ClearAllMeshSections();
			}

			FSimpleUnrealPakModule &SimpleUnrealPakModule = FModuleManager::LoadModuleChecked<FSimpleUnrealPakModule>(TEXT("SimpleUnrealPak"));
			UStaticMesh *Mesh = SimpleUnrealPakModule.StaticLoadPakObject<UStaticMesh>(AssetPaths);
			if (Mesh)
			{
				GMeshComponent->SetStaticMesh(Mesh);
			}
			UMaterialInterface *MaterialObject = SimpleUnrealPakModule.StaticLoadPakObject<UMaterialInterface>(AssetPaths);
			if (MaterialObject)
			{
				TArray<FVector> Vertices;
				TArray<int32> Triangles;
				TArray<FVector> Normals;
				TArray<FVector2D> UV;
				TArray<FColor> VertexColors;
				TArray<FProcMeshTangent> ProcMeshTangent;
				UKismetProceduralMeshLibrary::GenerateBoxMesh(FVector(100.0f), Vertices, Triangles, Normals, UV, ProcMeshTangent);
				UKismetProceduralMeshLibrary::CalculateTangentsForMesh( Vertices, Triangles, UV, Normals, ProcMeshTangent);
				GProceduralMeshComponent->CreateMeshSection(0, Vertices, Triangles, Normals, UV, VertexColors, ProcMeshTangent,false);
				GProceduralMeshComponent->SetMaterial(0, MaterialObject);
			}

			UTexture2D *Texture2DObject = SimpleUnrealPakModule.StaticLoadPakObject<UTexture2D>(AssetPaths);
			if(Texture2DObject)
			{

			}

			UBlueprint *Blueprint = SimpleUnrealPakModule.StaticLoadPakObject<UBlueprint>(AssetPaths);
			if (Blueprint && GetWorld())
			{
				AActor *A = GetWorld()->SpawnActor<AActor>(Blueprint->GeneratedClass, FVector::ZeroVector,FRotator::ZeroRotator);
				if (A)
				{
					Actors.Add(A);
				}
			}
		}
	}

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE