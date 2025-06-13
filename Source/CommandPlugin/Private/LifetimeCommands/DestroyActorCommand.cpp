// Copyright (c) 2024, Dane Sherman. All rights reserved.

#include "LifetimeCommands/DestroyActorCommand.h"

#include "CommandUtil.h"

void UDestroyActorCommand::Do_Implementation()
{
	if (ActorToDestroy == NULL) 
	{
		return;
	}

	InLimbo = true;
	CommandUtil::DisableActor(ActorToDestroy);
}

void UDestroyActorCommand::Undo_Implementation()
{
	if (ActorToDestroy == NULL) 
	{
		return;
	}

	InLimbo = false;
	CommandUtil::EnableActor(ActorToDestroy);
}

FString UDestroyActorCommand::GetDisplayString_Implementation()
{
	return "Destroy";
}

void UDestroyActorCommand::BeginDestroy()
{
	if (InLimbo && ActorToDestroy != NULL)
	{
		ActorToDestroy->Destroy();
	}

	Super::BeginDestroy();
}
