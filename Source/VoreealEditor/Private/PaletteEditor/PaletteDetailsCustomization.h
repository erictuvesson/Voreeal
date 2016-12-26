#pragma once

#include "PropertyEditing.h"
#include "PaletteEditorViewportClient.h"

class FPaletteDetailsCustomization : public IDetailCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstance();

	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstanceForEditor();

	// Begin IDetailCustomization Interface
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;
	// End IDetailCustomization Interface

protected:
	static FDetailWidgetRow& GenerateWarningRow(IDetailCategoryBuilder& WarningCategory, bool bExperimental, 
		const FText& WarningText, const FText& Tooltip, const FString& ExcerptLink, const FString& ExcerptName);

	// Creates a name widget with optional captioning label
	TSharedRef<SWidget> CreateTextureNameWidget(TSharedPtr<IPropertyHandle> PropertyHandle, const FText& OverrideText);
};
