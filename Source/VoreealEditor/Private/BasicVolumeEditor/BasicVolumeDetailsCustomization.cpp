#include "VoreealEditorPrivatePCH.h"
#include "BasicVolumeDetailsCustomization.h"

#include "Editor/Documentation/Public/IDocumentation.h"
#include "PropertyRestriction.h"
#include "PropertyCustomizationHelpers.h"

#define LOCTEXT_NAMESPACE "VoreealEditor"

TSharedRef<IDetailCustomization> FBasicVolumeDetailsCustomization::MakeInstance()
{
	TAttribute<EBasicVolumeEditorMode> DummyEditMode(EBasicVolumeEditorMode::ViewMode);
	return MakeInstanceForEditor(DummyEditMode);
}

TSharedRef<IDetailCustomization> FBasicVolumeDetailsCustomization::MakeInstanceForEditor(TAttribute<EBasicVolumeEditorMode> InEditMode)
{
	return MakeShareable(new FBasicVolumeDetailsCustomization(InEditMode));
}

FBasicVolumeDetailsCustomization::FBasicVolumeDetailsCustomization(TAttribute<EBasicVolumeEditorMode> InEditMode)
{
	VolumeEditMode = InEditMode;
}

FDetailWidgetRow& FBasicVolumeDetailsCustomization::GenerateWarningRow(IDetailCategoryBuilder& WarningCategory, bool bExperimental, const FText& WarningText, const FText& Tooltip, const FString& ExcerptLink, const FString& ExcerptName)
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
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.AutoWidth()
			.Padding(4.0f, 0.0f, 0.0f, 0.0f)
			[
				SNew(SImage)
				.Image(WarningIcon)
			]
			+ SHorizontalBox::Slot()
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

void FBasicVolumeDetailsCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
	// Build the socket category
	IDetailCategoryBuilder& SocketCategory = DetailLayout.EditCategory("Sockets");
	//SocketCategory.AddProperty(GET_MEMBER_NAME_CHECKED(UBasicVolume, Sockets));
}

EVisibility FBasicVolumeDetailsCustomization::EditorModeMatches(EBasicVolumeEditorMode DesiredMode) const
{
	return (VolumeEditMode.Get() == DesiredMode) ? EVisibility::Visible : EVisibility::Collapsed;
}

EVisibility FBasicVolumeDetailsCustomization::EditorModeIsNot(EBasicVolumeEditorMode DesiredMode) const
{
	return (VolumeEditMode.Get() != DesiredMode) ? EVisibility::Visible : EVisibility::Collapsed;
}

#undef LOCTEXT_NAMESPACE
