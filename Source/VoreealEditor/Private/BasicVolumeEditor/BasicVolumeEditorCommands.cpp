#include "VoreealEditorPrivatePCH.h"
#include "BasicVolumeEditorCommands.h"

#define LOCTEXT_NAMESPACE "VoreealEditor"

void FBasicVolumeEditorCommands::RegisterCommands()
{
	// Show toggles
	UI_COMMAND(SetShowGrid, "Grid", "Displays the viewport grid.", EUserInterfaceActionType::ToggleButton, FInputChord());

	UI_COMMAND(SetShowSockets, "Sockets", "Displays the volume sockets.", EUserInterfaceActionType::ToggleButton, FInputChord());

	UI_COMMAND(SetShowPivot, "Pivot", "Display the pivot location of the volume.", EUserInterfaceActionType::ToggleButton, FInputChord());

	// Editing modes
	UI_COMMAND(EnterViewMode, "View", "View the volume.", EUserInterfaceActionType::ToggleButton, FInputChord());
}

#undef LOCTEXT_NAMESPACE