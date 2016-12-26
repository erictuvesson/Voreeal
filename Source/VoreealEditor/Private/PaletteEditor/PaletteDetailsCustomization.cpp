#include "VoreealEditorPrivatePCH.h"
#include "PaletteDetailsCustomization.h"

#include "Editor/Documentation/Public/IDocumentation.h"
#include "PropertyRestriction.h"
#include "PropertyCustomizationHelpers.h"

#define LOCTEXT_NAMESPACE "VoreealEditor"

TSharedRef<IDetailCustomization> FPaletteDetailsCustomization::MakeInstance()
{
	// TODO: Remove
	return MakeInstanceForEditor();
}

TSharedRef<IDetailCustomization> FPaletteDetailsCustomization::MakeInstanceForEditor()
{
	return MakeShareable(new FPaletteDetailsCustomization());
}

FDetailWidgetRow& FPaletteDetailsCustomization::GenerateWarningRow(IDetailCategoryBuilder& WarningCategory, bool bExperimental, const FText& WarningText, const FText& Tooltip, const FString& ExcerptLink, const FString& ExcerptName)
{
	const FText SearchString = WarningText;
	const FSlateBrush* WarningIcon = FEditorStyle::GetBrush(bExperimental ? "PropertyEditor.ExperimentalClass" : "PropertyEditor.EarlyAccessClass");

	FDetailWidgetRow& WarningRow = WarningCategory
		.AddCustomRow(SearchString)
		.WholeRowContent()
		[
			SNew(SHorizontalBox)
			.ToolTip(IDocumentation::Get()->CreateToolTip(Tooltip, nullptr, ExcerptLink, ExcerptName))
			.Visibility(EVisibility::Visible)
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.AutoWidth()
			.Padding(4.0f, 0.0f, 0.0f, 0.0f)
			[
				SNew(SImage)
				.Image(WarningIcon)
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.AutoWidth()
			.Padding(4.0f, 0.0f, 0.0f, 0.0f)
			[
				SNew(STextBlock)
				.Text(WarningText)
				.Font(IDetailLayoutBuilder::GetDetailFont())
			]
		];

	return WarningRow;
}

void FPaletteDetailsCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{

}

#undef LOCTEXT_NAMESPACE
