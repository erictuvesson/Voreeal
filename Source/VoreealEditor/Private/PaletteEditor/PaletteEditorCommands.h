#pragma once

#include "VoreealStyle.h"

class FPaletteEditorCommands : public TCommands<FPaletteEditorCommands>
{
public:
	FPaletteEditorCommands();

	// Begin TCommand<> Interface
	virtual void RegisterCommands() override;
	// End TCommand<> Interface

};
