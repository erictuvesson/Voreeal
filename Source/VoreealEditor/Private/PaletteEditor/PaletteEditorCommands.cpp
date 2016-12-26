#include "VoreealEditorPrivatePCH.h"
#include "PaletteEditorCommands.h"

#define LOCTEXT_NAMESPACE "VoreealEditor"

FPaletteEditorCommands::FPaletteEditorCommands()
	: TCommands<FPaletteEditorCommands>(
		TEXT("PaletteEditor"), // Context name for fast lookup
		NSLOCTEXT("Contexts", "PaletteEditor", "Basic Volume Editor"), // Localized context name for displaying
		NAME_None, // Parent
		FVoreealStyle::Get()->GetStyleSetName() // Icon Style Set
		)
{

}

void FPaletteEditorCommands::RegisterCommands()
{

}

#undef LOCTEXT_NAMESPACE