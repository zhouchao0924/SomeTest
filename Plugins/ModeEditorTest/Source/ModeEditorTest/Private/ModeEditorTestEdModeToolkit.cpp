// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ModeEditorTestEdModeToolkit.h"
#include "ModeEditorTestEdMode.h"
#include "Engine/Selection.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorModeManager.h"
#include "ScopedTransaction.h"
#include "Engine/Polys.h"
#include "Model.h"
#include "Components/BrushComponent.h"
#include "Components/SkinnedMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Rendering/SkeletalMeshRenderData.h"
#include "Rendering/SkeletalMeshLODRenderData.h"

#define LOCTEXT_NAMESPACE "FModeEditorTestEdModeToolkit"
#define COMPARE_LOCTEXT(TextA,TextB)\
TextA.ToString() == TextB.ToString()

namespace LocalNode
{
	static FText LeftFaceBehindTopID = LOCTEXT("LeftFaceBehindTopID", "Left Face Behind Top Point");
}

FModeEditorTestEdModeToolkit::FModeEditorTestEdModeToolkit()
{
}

void FModeEditorTestEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	struct Locals
	{
		static bool IsWidgetEnabled()
		{
			return GEditor->GetSelectedActors()->Num() != 0;
		}

		static FReply OnButtonClickPointChange(FText InID)
		{
			if (GEditor->GetSelectedActorCount() <= 0)
			{
				return FReply::Unhandled();
			}

			const FScopedTransaction Transaction(LOCTEXT("ActorsPivotChange", "Actors Pivot Change"));

			for (FSelectionIterator It(GEditor->GetSelectedActorIterator()); It; ++It)
			{
				AActor *MyActor = static_cast<AActor*>(*It);
				FVector PivotPoint = GetSelectActorPivot(MyActor, InID);

				FVector Delta(PivotPoint - MyActor->GetActorLocation());
				PivotPoint = MyActor->GetTransform().InverseTransformVector(Delta);

				if (MyActor->GetPivotOffset() != PivotPoint)
				{
					MyActor->Modify();
					MyActor->SetPivotOffset(PivotPoint);
				}
			}

			GUnrealEd->NoteSelectionChange();

			return FReply::Handled();
		}

		static FVector GetSelectActorPivot(AActor *Actor, const FText &ID)
		{
			if (FMath::IsNearlyZero(Actor->GetComponentsBoundingBox().GetVolume(), KINDA_SMALL_NUMBER))
			{
				return Actor->GetActorLocation();
			}

			FVector Point(0);
			const FTransform ActorTransform = Actor->GetActorTransform();

			TArray<UPrimitiveComponent*> ValidComponents;
			GetValidComponents(Actor, ValidComponents);

			FBox MyBound(EForceInit::ForceInitToZero);
			if (ValidComponents.Num() == 1 &&
				Actor->GetRootComponent() == ValidComponents[0] &&
				GetComponentBox(ValidComponents[0], MyBound))
			{
				FVector RelativePoint = GetMyBoundingBoxPoint(MyBound, ID);
				Point = ActorTransform.TransformPosition(RelativePoint);
			}

			return Point;
		}

		static FVector GetMyBoundingBoxPoint(const FBox& InBoundingBox, FText ID)
		{
			FVector Result = FVector::ZeroVector;

			FVector Origin = InBoundingBox.GetCenter();
			FVector Extent = InBoundingBox.GetExtent();
			FVector Offset = FVector::ZeroVector;

			/////////////////////////////////////C/////////////////////////////////////
			if (COMPARE_LOCTEXT(ID, LocalNode::LeftFaceBehindTopID))
			{
				Offset = FVector(0, 0, Extent.Z);
			}

			Result = Origin + Offset;

			return Result;
		}

		static bool GetComponentBox(UPrimitiveComponent* InComponent, FBox& OutBox)
		{
			bool bBox = false;

			if (InComponent)
			{
				if (!bBox)
				{
					const UStaticMeshComponent* StaticMeshComponent = Cast<const UStaticMeshComponent>(InComponent);
					if (StaticMeshComponent != NULL && StaticMeshComponent->GetStaticMesh() != NULL)
					{
						OutBox = StaticMeshComponent->GetStaticMesh()->GetBoundingBox();
						return true;
					}
				}

				if (!bBox)
				{
					const UBrushComponent* BrushComponent = Cast<const UBrushComponent>(InComponent);
					if (BrushComponent != NULL && BrushComponent->Brush != NULL)
					{
						OutBox = BrushComponent->Brush->Bounds.GetBox();
						return true;
					}
				}

				if (!bBox)
				{
					const USkinnedMeshComponent* SkinnedMeshComponent = Cast<const USkinnedMeshComponent>(InComponent);
					if (SkinnedMeshComponent != NULL && SkinnedMeshComponent->SkeletalMesh != NULL)
					{
						OutBox = SkinnedMeshComponent->SkeletalMesh->GetImportedBounds().GetBox();
						return true;
					}
				}
			}

			return bBox;
		}

		static void GetValidComponents(AActor* MyActor, TArray<UPrimitiveComponent*>& MyComponents)
		{
			if (MyActor)
			{
				TArray<UPrimitiveComponent*> PrimitiveComponents;
				MyActor->GetComponents<UPrimitiveComponent>(PrimitiveComponents);

				for (UPrimitiveComponent* Component : PrimitiveComponents)
				{
					bool bValidMesh = false;

					if (!bValidMesh)
					{
						UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(Component);

						bValidMesh = (
							StaticMeshComponent &&
							StaticMeshComponent->GetStaticMesh() &&
							StaticMeshComponent->GetStaticMesh()->RenderData->LODResources[0].VertexBuffers.PositionVertexBuffer.GetNumVertices() > 0);
					}

					if (!bValidMesh)
					{
						UBrushComponent* BrushComponent = Cast<UBrushComponent>(Component);

						bValidMesh = (
							BrushComponent &&
							BrushComponent->Brush &&
							BrushComponent->Brush->Polys->Element.Num() > 0);
					}

					if (!bValidMesh)
					{
						USkinnedMeshComponent* SkinnedMeshComponent = Cast<USkinnedMeshComponent>(Component);

						bValidMesh = (
							SkinnedMeshComponent &&
							SkinnedMeshComponent->GetSkeletalMeshRenderData()->LODRenderData[0].RenderSections[0].NumVertices > 0);
					}

					if (bValidMesh)
					{
						MyComponents.Add(Component);
					}
				}
			}
		}

		static FReply OnButtonClick(FVector InOffset)
		{
			USelection* SelectedActors = GEditor->GetSelectedActors();

			// Let editor know that we're about to do something that we want to undo/redo
			GEditor->BeginTransaction(LOCTEXT("MoveActorsTransactionName", "MoveActors"));

			// For each selected actor
			for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
			{
				if (AActor* LevelActor = Cast<AActor>(*Iter))
				{
					// Register actor in opened transaction (undo/redo)
					LevelActor->Modify();
					// Move actor to given location
					LevelActor->TeleportTo(LevelActor->GetActorLocation() + InOffset, FRotator(0, 0, 0));
				}
			}

			// We're done moving actors so close transaction
			GEditor->EndTransaction();

			return FReply::Handled();
		}

		static TSharedRef<SWidget> MakeButton(FText InLabel, const FVector InOffset)
		{
			return SNew(SButton)
				.Text(InLabel)
				.OnClicked_Static(&Locals::OnButtonClick, InOffset);
		}

		static TSharedRef<SWidget> MakePointButton(FText InID)
		{
			return SNew(SButton)
				.Text(InID)
				.OnClicked_Static(&Locals::OnButtonClickPointChange, InID);
		}
	};

	const float Factor = 256.0f;

	SAssignNew(ToolkitWidget, SBorder)
		.HAlign(HAlign_Center)
		.Padding(25)
		.IsEnabled_Static(&Locals::IsWidgetEnabled)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			.Padding(50)
			[
				SNew(STextBlock)
				.AutoWrapText(true)
				.Text(LOCTEXT("HelperLabel", "Select some actors and move them around using buttons below"))
			]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					Locals::MakeButton(LOCTEXT("UpButtonLabel", "Up"), FVector(0, 0, Factor))
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						Locals::MakeButton(LOCTEXT("LeftButtonLabel", "Left"), FVector(0, -Factor, 0))
					]
					+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							Locals::MakeButton(LOCTEXT("RightButtonLabel", "Right"), FVector(0, Factor, 0))
						]
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					Locals::MakeButton(LOCTEXT("DownButtonLabel", "Down"), FVector(0, 0, -Factor))
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					Locals::MakePointButton(LocalNode::LeftFaceBehindTopID)
				]

		];
		
	FModeToolkit::Init(InitToolkitHost);
}

FName FModeEditorTestEdModeToolkit::GetToolkitFName() const
{
	return FName("ModeEditorTestEdMode");
}

FText FModeEditorTestEdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("ModeEditorTestEdModeToolkit", "DisplayName", "ModeEditorTestEdMode Tool");
}

class FEdMode* FModeEditorTestEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FModeEditorTestEdMode::EM_ModeEditorTestEdModeId);
}

#undef LOCTEXT_NAMESPACE
