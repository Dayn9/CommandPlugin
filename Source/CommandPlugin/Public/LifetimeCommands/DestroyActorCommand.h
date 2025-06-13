// Copyright (c) 2024, Dane Sherman. All rights reserved.
#pragma once

#include "Command.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "DestroyActorCommand.generated.h"

/**
 * Base blueprint command for destroying actors
 *
 * In order to prevent issues with object destruction and broken references,
*  when this command is done or redone the actor will NOT be immediately destroyed.
 * In the case where an actor is destroyed but has previous commands that hold references to it
 * we do not want to break those previous commands be deleting the actor they reference.
 * The actor will only be destroyed if this command is done and then removed from the command history.
 * This condition ensures that the actor will not be re-spawned (by undoing this command) and it is safe to delete.
 *
 */
UCLASS(BlueprintType, Blueprintable)
class COMMANDPLUGIN_API UDestroyActorCommand : public UObject, public ICommand
{
	GENERATED_BODY()

private:

	/* True when actor should be destroyed command is still in history */
	bool InLimbo = false;

public:

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
	TObjectPtr<AActor> ActorToDestroy = nullptr;

	void Do_Implementation() override;

	void Undo_Implementation() override;

	FString GetDisplayString_Implementation() override;

	virtual void BeginDestroy() override;
};
