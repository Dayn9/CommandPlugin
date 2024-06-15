// Copyright (c) 2024, Dane Sherman. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Command.h"
#include "MultiCommand.generated.h"

/* 
* Command for linking a group of commands to be done or undone all at once (in order)
* Use for sequences that must be done in order together (ex: object setup)
* Use for executing multiple of the same command at once (ex: move group command can be many move actor commands)
*/
UCLASS(BlueprintType)
class COMMANDPLUGIN_API UMultiCommand : public UObject, public ICommand
{
	GENERATED_BODY()

private:
	TArray<TScriptInterface<ICommand>> Commands = TArray<TScriptInterface<ICommand>>();

public:
	void Do_Implementation() override;
	void Undo_Implementation() override;
	FString GetDisplayString_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Command | MultiCommand")
	void Add(TScriptInterface<ICommand> AddCommand);
	
	UFUNCTION(BlueprintCallable, Category = "Command | MultiCommand")
	void Append(TArray<TScriptInterface<ICommand>> AddCommands);

	virtual void BeginDestroy() override;
};
