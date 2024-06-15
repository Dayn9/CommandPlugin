// Copyright (c) 2024, Dane Sherman. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "Command.h"

class CommandUtil
{
public:
	static void DestroyCommand(TScriptInterface<ICommand> Command);
};
