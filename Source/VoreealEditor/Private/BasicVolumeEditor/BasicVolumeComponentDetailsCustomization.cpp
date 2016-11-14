#include "VoreealEditorPrivatePCH.h"
#include "BasicVolumeComponentDetailsCustomization.h"

#include "BasicVolumeComponentDetailsCustomization.h"

#define LOCTEXT_NAMESPACE "VoreealEditor"

TSharedRef<IDetailCustomization> FBasicVolumeComponentDetailsCustomization::MakeInstance()
{
	return MakeShareable(new FBasicVolumeComponentDetailsCustomization);
}

void FBasicVolumeComponentDetailsCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	// Create a category so this is displayed early in the properties
	IDetailCategoryBuilder& BasicVolumeCategory = DetailBuilder.EditCategory("BasicVolume", FText::GetEmpty(), ECategoryPriority::Important);

	ObjectsBeingCustomized.Empty();
	DetailBuilder.GetObjectsBeingCustomized(/*out*/ ObjectsBeingCustomized);
}

#undef LOCTEXT_NAMESPACE