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
	//c
	static FText PointCenterID = LOCTEXT("PointCenter", "Center");//1
	//////////////////////////////Point////////////////////////////////////////////
	static FText PointAID = LOCTEXT("Point1", "Point 1");//1
	static FText PointBID = LOCTEXT("Point2", "Point 2");//2
	static FText PointCID = LOCTEXT("Point3", "Point 3");//3
	static FText PointDID = LOCTEXT("Point4", "Point 4");//4
	static FText PointEID = LOCTEXT("Point5", "Point 5");//5
	static FText PointFID = LOCTEXT("Point6", "Point 6");//6
	static FText PointGID = LOCTEXT("Point7", "Point 7");//7
	static FText PointHID = LOCTEXT("Point8", "Point 8");//8
	//////////////////////////////Line////////////////////////////////////////////
	static FText PointACID = LOCTEXT("Point13", "Point 1 and 3 center");//13
	static FText PointADID = LOCTEXT("Point14", "Point 1 and 4 center");//14
	static FText PointBGID = LOCTEXT("Point27", "Point 2 and 7 center");//27
	static FText PointAGID = LOCTEXT("Point17", "Point 1 and 7 center");//17
	static FText PointBDID = LOCTEXT("Point24", "Point 2 and 4 center");//24
	static FText PointDEID = LOCTEXT("Point45", "Point 5 and 4 center");//45
	static FText PointBFID = LOCTEXT("Point26", "Point 2 and 6 center");//26
	static FText PointGHID = LOCTEXT("Point78", "Point 7 and 8 center");//78
	static FText PointCHID = LOCTEXT("Point38", "Point 3 and 8 center");//38
	static FText PointCEID = LOCTEXT("Point35", "Point 3 and 5 center");//35
	static FText PointHFID = LOCTEXT("Point86", "Point 8 and 6 center");//86
	static FText PointEFID = LOCTEXT("Point56", "Point 5 and 6 center");//56
	////////////////////////////////Face//////////////////////////////////////////
	static FText PointFaceAID = LOCTEXT("PointFaceA", "FaceA ceentr");//56
	static FText PointFaceBID = LOCTEXT("PointFaceB", "FaceB ceentr");//56
	static FText PointFaceCID = LOCTEXT("PointFaceC", "FaceC ceentr");//56
	static FText PointFaceDID = LOCTEXT("PointFaceD", "FaceD ceentr");//56
	static FText PointFaceEID = LOCTEXT("PointFaceE", "FaceE ceentr");//56
	static FText PointFaceFID = LOCTEXT("PointFaceF", "FaceF ceentr");//56
}

FModeEditorTestEdModeToolkit::FModeEditorTestEdModeToolkit()
{
}

FVector PivotOffset = FVector::ZeroVector;

void FModeEditorTestEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	struct Locals
	{
		//复制中心点
		static FReply OnClickCopyPivot()
		{
			AActor* SelectedActor = GEditor->GetSelectedActors()->GetBottom<AActor>();
			if (SelectedActor)
			{
				PivotOffset = SelectedActor->GetPivotOffset();
			}

			return FReply::Handled();
		}

		//粘贴中心点
		static FReply OnClickPastePivot()
		{
			if (GEditor->GetSelectedActorCount() > 0)
			{
				const FScopedTransaction Transaction(LOCTEXT("PivotOffset", " Pivot Offset My Obj"));

				for (FSelectionIterator It(GEditor->GetSelectedActorIterator()); It; ++It)
				{
					AActor* Actor = static_cast<AActor*>(*It);
					if (Actor->GetPivotOffset() != PivotOffset)
					{
						Actor->Modify();
						Actor->SetPivotOffset(PivotOffset);
					}
				}

				GUnrealEd->NoteSelectionChange();
			}

			return FReply::Handled();
		}

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
			if (COMPARE_LOCTEXT(ID, LocalNode::PointCenterID))
			{
				Offset = FVector(FVector::ZeroVector);
			}

			///////////////////////////////////////P///////////////////////////////////
			else if (COMPARE_LOCTEXT(ID, LocalNode::PointAID))
			{
				Offset = FVector(-Extent.X, -Extent.Y, Extent.Z);
			}
			else if (COMPARE_LOCTEXT(ID, LocalNode::PointBID))
			{
				Offset = FVector(Extent.X, Extent.Y, Extent.Z);
			}
			else if (COMPARE_LOCTEXT(ID, LocalNode::PointCID))
			{
				Offset = FVector(-Extent.X, -Extent.Y, -Extent.Z);
			}
			else if (COMPARE_LOCTEXT(ID, LocalNode::PointDID))
			{
				Offset = FVector(-Extent.X, Extent.Y, Extent.Z);
			}
			else if (COMPARE_LOCTEXT(ID, LocalNode::PointEID))
			{
				Offset = FVector(-Extent.X, Extent.Y, -Extent.Z);
			}
			else if (COMPARE_LOCTEXT(ID, LocalNode::PointFID))
			{
				Offset = FVector(Extent.X, Extent.Y, -Extent.Z);
			}
			else if (COMPARE_LOCTEXT(ID, LocalNode::PointGID))
			{
				Offset = FVector(Extent.X, -Extent.Y, -Extent.Z);
			}
			else if (COMPARE_LOCTEXT(ID, LocalNode::PointHID))
			{
				Offset = FVector(Extent.X, -Extent.Y, Extent.Z);
			}

			//////////////////////////////////E////////////////////////////////////////
			else if (COMPARE_LOCTEXT(ID, LocalNode::PointACID))
			{
				Offset = FVector(-Extent.X, 0.0f, Extent.Z);//1
			}
			else if (COMPARE_LOCTEXT(ID, LocalNode::PointADID))
			{
				Offset = FVector(-Extent.X, 0.0f, -Extent.Z);
			}
			else if (COMPARE_LOCTEXT(ID, LocalNode::PointBGID))
			{
				Offset = FVector(0.0f, -Extent.Y, Extent.Z);//2
			}
			else if (COMPARE_LOCTEXT(ID, LocalNode::PointAGID))
			{
				Offset = FVector(0.0f, -Extent.Y, -Extent.Z);
			}
			else if (COMPARE_LOCTEXT(ID, LocalNode::PointBDID))
			{
				Offset = FVector(Extent.X, 0.0f, Extent.Z);//3
			}
			else if (COMPARE_LOCTEXT(ID, LocalNode::PointDEID))
			{
				Offset = FVector(Extent.X, 0.0f, -Extent.Z);
			}
			else if (COMPARE_LOCTEXT(ID, LocalNode::PointBFID))
			{
				Offset = FVector(0.0f, Extent.Y, Extent.Z);//4
			}
			else if (COMPARE_LOCTEXT(ID, LocalNode::PointGHID))
			{
				Offset = FVector(0.0f, Extent.Y, -Extent.Z);
			}
			else if (COMPARE_LOCTEXT(ID, LocalNode::PointCHID))
			{
				Offset = FVector(Extent.X, -Extent.Y, 0.0f);//5
			}
			else if (COMPARE_LOCTEXT(ID, LocalNode::PointCEID))
			{
				Offset = FVector(Extent.X, Extent.Y, 0.0f);
			}
			else if (COMPARE_LOCTEXT(ID, LocalNode::PointHFID))
			{
				Offset = FVector(-Extent.X, -Extent.Y, 0.0f);//6
			}
			else if (COMPARE_LOCTEXT(ID, LocalNode::PointEFID))
			{
				Offset = FVector(-Extent.X, Extent.Y, 0.0f);
			}

			//////////////////////////////////////////////////////////////////////////
			else if (COMPARE_LOCTEXT(ID, LocalNode::PointFaceAID))
			{
				Offset = FVector(-Extent.X, 0.0f, 0.0f);
			}
			else if (COMPARE_LOCTEXT(ID, LocalNode::PointFaceBID))
			{
				Offset = FVector(Extent.X, 0.0f, 0.0f);
			}
			else if (COMPARE_LOCTEXT(ID, LocalNode::PointFaceCID))
			{
				Offset = FVector(0.0f, Extent.Y, 0.0f);
			}
			else if (COMPARE_LOCTEXT(ID, LocalNode::PointFaceDID))
			{
				Offset = FVector(0.0f, -Extent.Y, 0.0f);
			}
			else if (COMPARE_LOCTEXT(ID, LocalNode::PointFaceEID))
			{
				Offset = FVector(0.0f, 0.0f, Extent.Z);
			}
			else if (COMPARE_LOCTEXT(ID, LocalNode::PointFaceFID))
			{
				Offset = FVector(0.0f, 0.0f, -Extent.Z);
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
			SNew(SScrollBox)
			+ SScrollBox::Slot()
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
			///////////////////////////////////////Center///////////////////////////////////
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					Locals::MakePointButton(LocalNode::PointCenterID)
				]
			//////////////////////////////////P////////////////////////////////////////
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Right)
				.AutoHeight()
				[
					Locals::MakePointButton(LocalNode::PointAID)
				]

			+ SVerticalBox::Slot()
				.HAlign(HAlign_Right)
				.AutoHeight()
				[
					Locals::MakePointButton(LocalNode::PointBID)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Right)
				.AutoHeight()
				[
					Locals::MakePointButton(LocalNode::PointCID)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Right)
				.AutoHeight()
				[
					Locals::MakePointButton(LocalNode::PointDID)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Right)
				.AutoHeight()
				[
					Locals::MakePointButton(LocalNode::PointEID)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Right)
				.AutoHeight()
				[
					Locals::MakePointButton(LocalNode::PointFID)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Right)
				.AutoHeight()
				[
					Locals::MakePointButton(LocalNode::PointGID)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Right)
				.AutoHeight()
				[
					Locals::MakePointButton(LocalNode::PointHID)
				]

			////////////////////////////////E//////////////////////////////////////////
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoHeight()
				[
					Locals::MakePointButton(LocalNode::PointACID)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoHeight()
				[
					Locals::MakePointButton(LocalNode::PointADID)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoHeight()
				[
					Locals::MakePointButton(LocalNode::PointBGID)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoHeight()
				[
					Locals::MakePointButton(LocalNode::PointAGID)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoHeight()
				[
					Locals::MakePointButton(LocalNode::PointBDID)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoHeight()
				[
					Locals::MakePointButton(LocalNode::PointDEID)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoHeight()
				[
					Locals::MakePointButton(LocalNode::PointBFID)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoHeight()
				[
					Locals::MakePointButton(LocalNode::PointGHID)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoHeight()
				[
					Locals::MakePointButton(LocalNode::PointCHID)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoHeight()
				[
					Locals::MakePointButton(LocalNode::PointCEID)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoHeight()
				[
					Locals::MakePointButton(LocalNode::PointHFID)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Left)
				.AutoHeight()
				[
					Locals::MakePointButton(LocalNode::PointEFID)
				]

			////////////////////////////////F//////////////////////////////////////////
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Right)
				.AutoHeight()
				[
					Locals::MakePointButton(LocalNode::PointFaceAID)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Right)
				.AutoHeight()
				[
					Locals::MakePointButton(LocalNode::PointFaceBID)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Right)
				.AutoHeight()
				[
					Locals::MakePointButton(LocalNode::PointFaceCID)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Right)
				.AutoHeight()
				[
					Locals::MakePointButton(LocalNode::PointFaceDID)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Right)
				.AutoHeight()
				[
					Locals::MakePointButton(LocalNode::PointFaceEID)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Right)
				.AutoHeight()
				[
					Locals::MakePointButton(LocalNode::PointFaceFID)
				]
		//////////////////////////////////////////////////////////////////////////
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SButton)
					.Text(LOCTEXT("CopyButtonPivot","Copy Pivot"))
					.OnClicked_Static(&Locals::OnClickCopyPivot)
				]
			+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SButton)
					.Text(LOCTEXT("PasteButtonPivot", "Paste Pivot"))
					.OnClicked_Static(&Locals::OnClickPastePivot)
				]

		]];
		
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
