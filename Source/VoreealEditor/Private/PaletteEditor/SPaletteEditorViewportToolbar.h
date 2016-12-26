#pragma once

#include "Editor/UnrealEd/Public/SCommonEditorViewportToolbarBase.h"

// In-viewport toolbar widget used in the editor
class SPaletteEditorViewportToolbar : public SCommonEditorViewportToolbarBase
{
public:
	SLATE_BEGIN_ARGS(SPaletteEditorViewportToolbar) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedPtr<class ICommonEditorViewportToolbarInfoProvider> InInfoProvider);

	// Begin SCommonEditorViewportToolbarBase Interface
	virtual TSharedRef<SWidget> GenerateShowMenu() const override;
	// End SCommonEditorViewportToolbarBase Interface
};
