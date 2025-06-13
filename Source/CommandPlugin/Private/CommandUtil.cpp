// Copyright (c) 2024, Dane Sherman. All rights reserved.

#include "CommandUtil.h"

void CommandUtil::DestroyCommand(TScriptInterface<ICommand> Command)
{
	UObject* CommandObject = Command.GetObject();
	if (CommandObject->IsValidLowLevel()) 
	{
		CommandObject->ConditionalBeginDestroy();
	}

	//DestructItem(&Command); //TODO test this as alternative
}

void CommandUtil::EnableActor(TObjectPtr<AActor> Actor)
{
	Actor->SetActorHiddenInGame(false);
	Actor->SetActorEnableCollision(true);
}

void CommandUtil::DisableActor(TObjectPtr<AActor> Actor)
{
	Actor->SetActorHiddenInGame(true);
	Actor->SetActorEnableCollision(false);
}
