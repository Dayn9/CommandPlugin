#include "CommandUtil.h"

CommandUtil::CommandUtil()
{
}

CommandUtil::~CommandUtil()
{
}

void CommandUtil::DestroyCommand(TScriptInterface<ICommand> Command)
{
	//DestructItem(&Command); //TODO test this as alternative

	UObject* CommandObject = Command.GetObject();
	if (CommandObject->IsValidLowLevel()) {
		CommandObject->ConditionalBeginDestroy();
	}
}
