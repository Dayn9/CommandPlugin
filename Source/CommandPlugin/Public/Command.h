// Copyright (c) 2024, Dane Sherman. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "Command.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UCommand : public UInterface
{
	GENERATED_BODY()
};

/* Trackable action that can be done or redone */
class COMMANDPLUGIN_API ICommand
{
	GENERATED_BODY()

public:

	/* changes state to new value. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Command")
	void Do();

	/* restores state to old value. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Command")
	void Undo();

	/* get a user display string for this command. 
	this should always reflect the new value */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Command")
	FString GetDisplayString();
};
