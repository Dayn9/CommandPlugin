// Copyright (c) 2024, Dane Sherman. All rights reserved.

#pragma once

#include "Command.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "SpawnActorCommand.generated.h"

/**
 * Base blueprint command for spawning actors
 * 
 * In order to prevent issues with object destruction and broken references, 
*  when this command is undone the spawned actor will NOT be immediately destroyed. 
 * In the case where an actor is spawned and then addition commands hold references to it
 * we do not want to break those later commands be deleting the actor they reference. 
 * The spawned actor will only be destroyed if this command is undone and then removed from the command history. 
 * This condition ensures that the spawned actor will not be re-spawned and it is safe to delete.
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class COMMANDPLUGIN_API USpawnActorCommand : public UObject, public ICommand
{
	GENERATED_BODY()

private:

	/* True when actor should be destroyed command is still in history */
	bool InLimbo = false;

	TObjectPtr<AActor> SpawnedActor = nullptr;

protected:

	UFUNCTION(BlueprintNativeEvent)
	AActor* SpawnActor();

public:

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FTransform SpawnTransform;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	ESpawnActorCollisionHandlingMethod CollisionHandlingOverride;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	ESpawnActorScaleMethod TransformScaleMethod;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	TObjectPtr<AActor> Owner;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const AActor* GetSpawnedActor();

	void Do_Implementation() override;

	void Undo_Implementation() override;

	FString GetDisplayString_Implementation() override;

	virtual void BeginDestroy() override;
	
};
