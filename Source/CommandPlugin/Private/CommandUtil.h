#pragma once

#include "CoreMinimal.h"
#include "Command.h"

class CommandUtil
{
public:
	CommandUtil();
	~CommandUtil();

	static void DestroyCommand(TScriptInterface<ICommand> Command);
};
