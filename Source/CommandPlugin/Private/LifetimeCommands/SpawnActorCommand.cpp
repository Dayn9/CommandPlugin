// Copyright (c) 2024, Dane Sherman. All rights reserved.

#include "LifetimeCommands/SpawnActorCommand.h"

#include "CommandUtil.h"

AActor* USpawnActorCommand::SpawnActor_Implementation()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = CollisionHandlingOverride;
	SpawnParams.TransformScaleMethod = TransformScaleMethod;
	SpawnParams.Owner = Owner;
	//SpawnParams.Instigator = Instigator; deprecated

	return GetWorld()->SpawnActor(ActorClass.Get(), &SpawnTransform, SpawnParams);
}

const AActor* USpawnActorCommand::GetSpawnedActor()
{
	return SpawnedActor;
}

void USpawnActorCommand::Do_Implementation()
{
	if (SpawnedActor == NULL) 
	{
		SpawnedActor = SpawnActor();
	}
	
	if (SpawnedActor == NULL) 
	{
		return;
	}

	InLimbo = false;
	CommandUtil::EnableActor(SpawnedActor);
}

void USpawnActorCommand::Undo_Implementation()
{
	if (SpawnedActor == NULL) 
	{
		return;
	}

	InLimbo = true;
	CommandUtil::DisableActor(SpawnedActor);
}

FString USpawnActorCommand::GetDisplayString_Implementation()
{
	return "Spawned";
}

void USpawnActorCommand::BeginDestroy()
{
	if (InLimbo && SpawnedActor != NULL)
	{
		SpawnedActor->Destroy();
	}

	Super::BeginDestroy();
}
