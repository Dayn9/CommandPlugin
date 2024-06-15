// Copyright Dane Sherman (danemsherman@gmail.com)
//
// Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
// or copy at http://opensource.org/licenses/MIT)

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
	/* changes state to new value. Returns true when state is changed */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Command")
	void Do();

	/* restores state to old value. Returns true when state is changed */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Command")
	void Undo();

	/* get a user display string for this command */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Command")
	FString GetDisplayString();
};
