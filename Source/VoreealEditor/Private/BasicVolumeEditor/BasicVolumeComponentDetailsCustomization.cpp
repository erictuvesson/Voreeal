#include "VoreealEditorPrivatePCH.h"
#include "BasicVolumeComponentDetailsCustomization.h"

#define LOCTEXT_NAMESPACE "VoreealEditor"

TSharedRef<IDetailCustomization> FBasicVolumeComponentDetailsCustomization::MakeInstance()
{
	return MakeShareable(new FBasicVolumeComponentDetailsCustomization);
}

void FBasicVolumeComponentDetailsCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	const FText ConvertToStaticMeshText = LOCTEXT("ConvertToStaticMesh", "Create StaticMesh");

	// Create a category so this is displayed early in the properties
	IDetailCategoryBuilder& BasicVolumeCategory = DetailBuilder.EditCategory("BasicVolume", FText::GetEmpty(), ECategoryPriority::Important);

	ObjectsBeingCustomized.Empty();
	DetailBuilder.GetObjectsBeingCustomized(/*out*/ ObjectsBeingCustomized);

	BasicVolumeCategory.AddCustomRow(ConvertToStaticMeshText, false)
		.NameContent()
		[
			SNullWidget::NullWidget
		]
		.ValueContent()
		.VAlign(VAlign_Center)
		.MaxDesiredWidth(250)
		[
			SNew(SButton)
			.VAlign(VAlign_Center)
			.ToolTipText(LOCTEXT("ConvertToStaticMeshTooltip", "Create a new StaticMesh asset using current voxels from this volume. Does not modify instance."))
			.OnClicked(this, &FBasicVolumeComponentDetailsCustomization::ClickedOnConvertToStaticMesh)
			.IsEnabled(this, &FBasicVolumeComponentDetailsCustomization::ConvertToStaticMeshEnabled)
			.Content()
			[
				SNew(STextBlock)
				.Text(ConvertToStaticMeshText)
			]
		];
}

bool FBasicVolumeComponentDetailsCustomization::ConvertToStaticMeshEnabled() const
{
	return false;
}

FReply FBasicVolumeComponentDetailsCustomization::ClickedOnConvertToStaticMesh()
{
	// TODO: Implement
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE