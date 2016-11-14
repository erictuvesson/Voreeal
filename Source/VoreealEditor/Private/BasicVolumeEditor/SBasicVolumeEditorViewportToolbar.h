#pragma once

#include "Editor/UnrealEd/Public/SCommonEditorViewportToolbarBase.h"

// In-viewport toolbar widget used in the editor
class SBasicVolumeEditorViewportToolbar : public SCommonEditorViewportToolbarBase
{
public:
	SLATE_BEGIN_ARGS(SBasicVolumeEditorViewportToolbar) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedPtr<class ICommonEditorViewportToolbarInfoProvider> InInfoProvider);

	// Begin SCommonEditorViewportToolbarBase Interface
	virtual TSharedRef<SWidget> GenerateShowMenu() const override;
	// End SCommonEditorViewportToolbarBase Interface
};
