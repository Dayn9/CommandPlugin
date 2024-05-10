#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Command.h"
#include "CommandHistoryExperimental.generated.h"

/*
	Linear history of commands that have been done and undone

	Commands Enter the history by Push(), which will call Do
	Commands Exit the history by MaxSize check or Clear() -> Destroyed
	While in the history, commands that are undone or redone will still be tracked

	Note: Calling Push() when there are undone commands will clear all undone commands from the history
*/
UCLASS(BlueprintType)
class COMMANDPLUGIN_API UCommandHistoryExperimental : public UObject
{
	GENERATED_BODY()

	/*
	This implementation was created to avoid Array reallocation, so indicies loop

	Basically treat CurrentIndex and LastIndex like usual where 0 is the first and oldest element
	When max size is reached, instead of destroying index 0 and shifting the whole array:
		we destroy index 0 and shift the starting index to 1. 
		The newest command can then be placed at index 0, but can still be referenced as index (max size - 1)

	IMPORTANT: Anytime the History is accessed, we convert to whatever the true index is
	*/

private: 
	UPROPERTY(BlueprintGetter = GetMaxSize, meta = (ExposeOnSpawn = true))
	int MaxSize = 128;

	int CurrentIndex = -1; // Index of the latest done command
	int LastIndex = -1; // Index of the latest command (CurrentIndex + the number of undone commands) 
	int IndexOffset = 0; // array index wrapping offset

	TArray<TScriptInterface<ICommand>> History = TArray<TScriptInterface<ICommand>>();

private: 
	/* Convert from History Index, to Array Index */
	inline int TrueIndex(int Index) { return (Index + IndexOffset) % MaxSize; }

	/* Unvalidated logic for performing undo*/
	void Undo_Impl();

	/* Unvalidated logic for performing redo*/
	void Redo_Impl();

	/* Clear Redoable and Destroy command */
	void ClearRedoable();

public: 
	UCommandHistoryExperimental();

	/* The maximum number of commands to track. older items will be deleted to make room for new ones*/
	UFUNCTION(BlueprintGetter, BlueprintPure)
	inline int GetMaxSize() const { return MaxSize; }

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
};
