// Copyright (c) {Year}, {CompanyName}. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Command.h"
#include "SpawnActorCommand.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class COMMANDPLUGIN_API USpawnActorCommand : public UObject, public ICommand
{
	GENERATED_BODY()

private:

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

	void Do_Implementation() override;
	void Undo_Implementation() override;
	FString GetDisplayString_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const AActor* GetSpawnedActor();

	virtual void BeginDestroy() override;
	
};
