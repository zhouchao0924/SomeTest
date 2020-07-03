#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"
#include "CustomAsset.h"

class FMeshlImportDetail : public IPropertyTypeCustomization
{
public:

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;

	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

private:
	FReply HandleImportClicked();

	void OnLabelsRootTextCommited(const FText& InText, ETextCommit::Type InCommitType) const;

	TSharedPtr<class SEditableTextBox> EditableTextBox;

	TSharedPtr<IPropertyHandle> HandleMeshSavePaths;

};