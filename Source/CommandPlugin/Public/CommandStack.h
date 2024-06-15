// Copyright (c) 2024, Dane Sherman. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Command.h"
#include "CommandStack.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UCommandStack : public UInterface
{
	GENERATED_BODY()
};

/*
	Base interface for a LINEAR history of commands that can be done and undone in sequence
*/
class COMMANDPLUGIN_API ICommandStack
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Command | CommandStack ")
	int GetMaxSize();

	/* Execute a command (Do) and begin track as the most recently done */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Command | CommandStack ")
	void Push(const TScriptInterface<ICommand>& Command);

	/* Returns true when there is history that can be undone */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Command | CommandStack ")
	bool CanUndo();
	/* Undo the most recently done command */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Command | CommandStack ")
	void Undo();
	/* Undo a set number of the most recently done commands */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Command | CommandStack ")
	void UndoNum(int num);
	/* Undo every command that has been done */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Command | CommandStack ")
	void UndoAll();

	/* Returns true when there is history that can be redone */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Command | CommandStack ")
	bool CanRedo();
	/* Redo the most recently undone command */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Command | CommandStack ")
	void Redo();
	/* Redo a set number of the most recently undone commands */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Command | CommandStack ")
	void RedoNum(int num);
	/* Redo every command that has been undone */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Command | CommandStack ")
	void RedoAll();

	/* Discard all history */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Command | CommandStack ")
	void Clear();
};
