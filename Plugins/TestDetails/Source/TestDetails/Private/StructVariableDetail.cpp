#include "StructVariableDetail.h"
#include "DetailWidgetRow.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SEditableTextBox.h"

void FStructVariableDetail::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	HeaderRow
	.NameContent()
	[
		PropertyHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	.MinDesiredWidth(125.0f)
	.MaxDesiredWidth(325.0f)
	[
		SAssignNew(EditableTextBox, SEditableTextBox)
		.OnTextCommitted(this, &FStructVariableDetail::OnPropertyTextCommited)
	];
}

void FStructVariableDetail::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	PropertyText = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FStructVariable, MyText));
	if (PropertyText && EditableTextBox.IsValid())
	{
		FText EventValueStr;
		PropertyText->GetValue(EventValueStr);

		if (EditableTextBox.IsValid())
		{
			EditableTextBox->SetText(EventValueStr);
		}
	}
}

TSharedRef<IPropertyTypeCustomization> FStructVariableDetail::MakeInstance()
{
	return MakeShareable(new FStructVariableDetail);
}

void FStructVariableDetail::OnPropertyTextCommited(const FText& InText, ETextCommit::Type InCommitType) const
{
	if (PropertyText.IsValid())
	{
		//获取路径值
		FText EventValueStr;
		PropertyText->GetValue(EventValueStr);

		PropertyText->SetValue(InText);
	}
}

