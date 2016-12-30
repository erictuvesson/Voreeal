#pragma once

#include "PropertyEditing.h"

class FBasicVolumeComponentDetailsCustomization : public IDetailCustomization
{
public:
	// Makes a new instance of this detail layout class for a specific detail view requesting it.
	static TSharedRef<IDetailCustomization> MakeInstance();

	// Begin IDetailCustomization Interface
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;
	// End IDetailCustomization Interface

	bool ConvertToStaticMeshEnabled() const;
	FReply ClickedOnConvertToStaticMesh();

protected:
	TArray<TWeakObjectPtr<UObject>> ObjectsBeingCustomized;
};
