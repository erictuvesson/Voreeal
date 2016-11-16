#include "VoreealEditorPrivatePCH.h"
#include "SBasicVolumeEditorViewportToolbar.h"

#include "BasicVolumeEditorCommands.h"
#include "SEditorViewport.h"

#define LOCTEXT_NAMESPACE "SBasicVolumeEditorViewportToolbar"

void SBasicVolumeEditorViewportToolbar::Construct(const FArguments& InArgs, TSharedPtr<class ICommonEditorViewportToolbarInfoProvider> InInfoProvider)
{
	SCommonEditorViewportToolbarBase::Construct(SCommonEditorViewportToolbarBase::FArguments(), InInfoProvider);
}

TSharedRef<SWidget> SBasicVolumeEditorViewportToolbar::GenerateShowMenu() const
{
	GetInfoProvider().OnFloatingButtonClicked();

	TSharedRef<SEditorViewport> ViewportRef = GetInfoProvider().GetViewportWidget();

	const bool bInShouldCloseWindowAfterMenuSelection = true;
	FMenuBuilder ShowMenuBuilder(bInShouldCloseWindowAfterMenuSelection, ViewportRef->GetCommandList());
	{
		ShowMenuBuilder.AddMenuEntry(FBasicVolumeEditorCommands::Get().SetShowSockets);
		ShowMenuBuilder.AddMenuEntry(FBasicVolumeEditorCommands::Get().SetShowPivot);

		ShowMenuBuilder.AddMenuSeparator();

		ShowMenuBuilder.AddMenuEntry(FBasicVolumeEditorCommands::Get().SetShowGrid);
		ShowMenuBuilder.AddMenuEntry(FBasicVolumeEditorCommands::Get().SetShowBounds);
	}

	return ShowMenuBuilder.MakeWidget();
}

#undef LOCTEXT_NAMESPACE
