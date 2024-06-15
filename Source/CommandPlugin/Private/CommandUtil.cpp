// Copyright (c) 2024, Dane Sherman. All rights reserved.
#include "CommandUtil.h"

void CommandUtil::DestroyCommand(TScriptInterface<ICommand> Command)
{
	//DestructItem(&Command); //TODO test this as alternative

	UObject* CommandObject = Command.GetObject();
	if (CommandObject->IsValidLowLevel()) {
		CommandObject->ConditionalBeginDestroy();
	}
}
