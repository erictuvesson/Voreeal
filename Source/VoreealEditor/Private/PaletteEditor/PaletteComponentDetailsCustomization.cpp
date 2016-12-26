#include "VoreealEditorPrivatePCH.h"
#include "PaletteComponentDetailsCustomization.h"

#define LOCTEXT_NAMESPACE "VoreealEditor"

TSharedRef<IDetailCustomization> FPaletteComponentDetailsCustomization::MakeInstance()
{
	return MakeShareable(new FPaletteComponentDetailsCustomization);
}

void FPaletteComponentDetailsCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	// Create a category so this is displayed early in the properties
	IDetailCategoryBuilder& PaletteCategory = DetailBuilder.EditCategory("Palette", FText::GetEmpty(), ECategoryPriority::Important);

	ObjectsBeingCustomized.Empty();
	DetailBuilder.GetObjectsBeingCustomized(/*out*/ ObjectsBeingCustomized);
}

#undef LOCTEXT_NAMESPACE