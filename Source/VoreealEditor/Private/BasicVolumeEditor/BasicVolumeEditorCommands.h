#pragma once

#include "VoreealStyle.h"

class FBasicVolumeEditorCommands : public TCommands<FBasicVolumeEditorCommands>
{
public:
	FBasicVolumeEditorCommands()
		: TCommands<FBasicVolumeEditorCommands>(
			TEXT("BasicVolumeEditor"), // Context name for fast lookup
			NSLOCTEXT("Contexts", "BasicVolumeEditor", "Basic Volume Editor"), // Localized context name for displaying
			NAME_None, // Parent
			FVoreealStyle::Get()->GetStyleSetName() // Icon Style Set
			)
	{

	}

	// Begin TCommand<> Interface
	virtual void RegisterCommands() override;
	// End TCommand<> Interface

public:
	// Show toggles
	TSharedPtr<FUICommandInfo> SetShowSockets;
	TSharedPtr<FUICommandInfo> SetShowPivot;

	TSharedPtr<FUICommandInfo> SetShowGrid;
	TSharedPtr<FUICommandInfo> SetShowBounds;
	TSharedPtr<FUICommandInfo> SetShowOctree;

	// Editing modes
	TSharedPtr<FUICommandInfo> EnterViewMode;
	TSharedPtr<FUICommandInfo> EnterEditMode;
};
