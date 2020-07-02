#include "MyTestDetailCustomization.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "PropertyCustomizationHelpers.h"
#include "Widgets/Images/SImage.h"

#define LOCTEXT_NAMESPACE "MyTestDetailCustomization"

void FMyTestDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& DetailCategoryBuilderInstance = DetailBuilder.EditCategory(TEXT("CustomizeDetails"));
	TSharedPtr<IPropertyHandle> HelloI = DetailBuilder.GetProperty("HelloI");
	DetailBuilder.AddPropertyToCategory(HelloI);
	DetailCategoryBuilderInstance
		.AddCustomRow(LOCTEXT("MMMM", "Hello Wolrd"))
		.WholeRowContent()
		[
			/*SNew(STextBlock)
			.Text(LOCTEXT("TTTT", "I am World"))*/

			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			[
				SNew(SProperty, HelloI)
				//.CustomWidget()
				//[
				//	SNew(SImage)
				//]
			]

			+ SHorizontalBox::Slot()
			[
				SNew(SProperty, DetailBuilder.GetProperty("HelloF"))
			]
		];
//	DetailCategoryBuilderInstance.AddProperty();
}

TSharedRef< IDetailCustomization > FMyTestDetailCustomization::MakeInstance()
{
	return MakeShareable(new FMyTestDetailCustomization);
}

#undef  LOCTEXT_NAMESPACE