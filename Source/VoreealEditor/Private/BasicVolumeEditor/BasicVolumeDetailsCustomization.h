#pragma once

#include "PropertyEditing.h"
#include "BasicVolumeEditorViewportClient.h"

class FBasicVolumeDetailsCustomization : public IDetailCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstance();

	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstanceForEditor(TAttribute<EBasicVolumeEditorMode> InEditMode);

	// Begin IDetailCustomization Interface
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;
	// End IDetailCustomization Interface

protected:
	FBasicVolumeDetailsCustomization(TAttribute<EBasicVolumeEditorMode> InEditMode);

	EVisibility EditorModeMatches(EBasicVolumeEditorMode DesiredMode) const;
	EVisibility EditorModeIsNot(EBasicVolumeEditorMode DesiredMode) const;

	static FDetailWidgetRow& GenerateWarningRow(IDetailCategoryBuilder& WarningCategory, bool bExperimental, 
		const FText& WarningText, const FText& Tooltip, const FString& ExcerptLink, const FString& ExcerptName);

	// Creates a name widget with optional captioning label
	TSharedRef<SWidget> CreateTextureNameWidget(TSharedPtr<IPropertyHandle> PropertyHandle, const FText& OverrideText);

protected:
	TAttribute<EBasicVolumeEditorMode> VolumeEditMode;
};
