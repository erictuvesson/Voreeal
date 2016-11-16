#include "VoreealEditorPrivatePCH.h"
#include "BasicVolumeEditorCommands.h"

#define LOCTEXT_NAMESPACE "VoreealEditor"

FBasicVolumeEditorCommands::FBasicVolumeEditorCommands()
	: TCommands<FBasicVolumeEditorCommands>(
		TEXT("BasicVolumeEditor"), // Context name for fast lookup
		NSLOCTEXT("Contexts", "BasicVolumeEditor", "Basic Volume Editor"), // Localized context name for displaying
		NAME_None, // Parent
		FVoreealStyle::Get()->GetStyleSetName() // Icon Style Set
		)
{

}

void FBasicVolumeEditorCommands::RegisterCommands()
{
	// Show toggles
	UI_COMMAND(SetShowSockets, "Sockets", "Displays the volume sockets.", EUserInterfaceActionType::ToggleButton, FInputChord());
	UI_COMMAND(SetShowPivot, "Pivot", "Display the pivot location of the volume.", EUserInterfaceActionType::ToggleButton, FInputChord());

	UI_COMMAND(SetShowGrid, "Grid", "Displays the viewport grid.", EUserInterfaceActionType::ToggleButton, FInputChord());
	UI_COMMAND(SetShowBounds, "Bounds", "Displays the volume bounds.", EUserInterfaceActionType::ToggleButton, FInputChord());
	UI_COMMAND(SetShowOctree, "Octree", "Displays the volume's octree.", EUserInterfaceActionType::ToggleButton, FInputChord());

	// Editing modes
	UI_COMMAND(EnterViewMode, "View", "View the volume.", EUserInterfaceActionType::ToggleButton, FInputChord());
	UI_COMMAND(EnterEditMode, "Edit", "Edit the volume.", EUserInterfaceActionType::ToggleButton, FInputChord());
}

#undef LOCTEXT_NAMESPACE