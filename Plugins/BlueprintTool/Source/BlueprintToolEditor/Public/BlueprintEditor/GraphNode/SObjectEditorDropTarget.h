#pragma once
#include "SAssetDropTarget.h"

class SObjectEditorDropTarget : public SAssetDropTarget
{
public:
	virtual FReply OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override
	{
		PanelCoordDropPosition = MyGeometry.AbsoluteToLocal(DragDropEvent.GetScreenSpacePosition());
		return SAssetDropTarget::OnDrop(MyGeometry, DragDropEvent);
	}

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override
	{
		if (GetDragOverlayVisibility().IsVisible())
		{
			SetVisibility(EVisibility::Visible);
		}
		else
		{
			SetVisibility(EVisibility::SelfHitTestInvisible);
		}
	}

	FVector2D GetPanelCoordDropPosition() const { return PanelCoordDropPosition; }
private:
	FVector2D PanelCoordDropPosition;
};