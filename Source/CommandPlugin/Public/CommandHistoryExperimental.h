#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Command.h"
#include "CommandHistoryExperimental.generated.h"

UCLASS(BlueprintType)
class COMMANDPLUGIN_API UCommandHistoryExperimental : public UObject
{
	GENERATED_BODY()
private:

	int MaxSize = 100;
	int FirstIndex = -1; //Index BEFORE any commands
	int CurrentIndex = -1; //Index of the latest done command
	int LastIndex = -1; 

	TArray<TScriptInterface<ICommand>> History = TArray<TScriptInterface<ICommand>>();
public: 
	/* execute and track command in history*/
	UFUNCTION(BlueprintCallable, Category = "Command | History")
	void Push(TScriptInterface<ICommand> Command);

	/* Returns true when there is history that can be undone */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Command | History")
	bool CanUndo();
	/* Undo the most recently done command */
	UFUNCTION(BlueprintCallable, Category = "Command | History")
	void Undo();
	/* Undo a set number of the most recently done commands */
	UFUNCTION(BlueprintCallable, Category = "Command | History")
	void UndoNum(int num);
	/* Undo every command that has been done */
	UFUNCTION(BlueprintCallable, Category = "Command | History")
	void UndoAll();

	/* Returns true when there is history that can be redone */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Command | History")
	bool CanRedo();
	/* Redo the most recently undone command */
	UFUNCTION(BlueprintCallable, Category = "Command | History")
	void Redo();
	/* Redo a set number of the most recently undone commands */
	UFUNCTION(BlueprintCallable, Category = "Command | History")
	void RedoNum(int num);
	/* Redo every command that has been undone */
	UFUNCTION(BlueprintCallable, Category = "Command | History")
	void RedoAll();

	/* Discard all history */
	UFUNCTION(BlueprintCallable, Category = "Command | History")
	void Clear();

private:
	/* Unvalidated logic for performing undo*/
	void UndoLatest();

	/* Unvalidated logic for performing redo*/
	void RedoLatest();

	/* Clear Redoable and Destroy command */
	void ClearRedoable();
};
