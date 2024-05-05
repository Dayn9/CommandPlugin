#include "CommandUtil.h"

CommandUtil::CommandUtil()
{
}

CommandUtil::~CommandUtil()
{
}

void CommandUtil::DestroyCommand(TScriptInterface<ICommand> Command)
{
	UObject* CommandObject = Command.GetObject();
	if (CommandObject->IsValidLowLevel()) {
		CommandObject->ConditionalBeginDestroy();
	}
}
