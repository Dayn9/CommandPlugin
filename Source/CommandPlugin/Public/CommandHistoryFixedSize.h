// Copyright (c) 2024, Dane Sherman. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Command.h"
#include "CommandStack.h"
#include "CommandHistoryFixedSize.generated.h"

/*
	Use this version of command history if you need to frequently access the command history
		- frequently pushing / undoing / redoing
		- frequently pushing at max size
		- this version does not resize, and will always allocate the MAX SIZE 

	Linear history of commands that have been done and undone

	Commands Enter the history by Push(), which will call Do
	Commands Exit the history by MaxSize check or Clear() -> Destroyed
	While in the history, commands that are undone or redone will still be tracked

	Note: Calling Push() when there are undone commands will clear all undone commands from the history
*/
UCLASS(BlueprintType)
class COMMANDPLUGIN_API UCommandHistoryFixedSize : public UObject, public ICommandStack
{
	GENERATED_BODY()

	/*
	This implementation was created to avoid Array reallocation, so the array index will loop

	Basically treat CurrentIndex and LastIndex like usual where 0 is the first and oldest element
	When max size is reached, instead of destroying index 0 and shifting the whole array:
		we destroy index 0 and shift the starting index to 1. 
		The newest command can then be placed at index 0, but can still be referenced as index (max size - 1)

	IMPORTANT: Anytime the History is accessed, we convert to whatever the true index is
	*/

public: 
	UCommandHistoryFixedSize();

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	int MaxSize = 100;

#pragma region  ICommandStack implementation

	int GetMaxSize_Implementation() override;

	void Push_Implementation(const TScriptInterface<ICommand>& Command) override;

	bool CanUndo_Implementation() override;

	void Undo_Implementation() override;

	void UndoNum_Implementation(int num) override;

	void UndoAll_Implementation() override;

	bool CanRedo_Implementation() override;

	void Redo_Implementation() override;

	void RedoNum_Implementation(int num) override;

	void RedoAll_Implementation() override;

	void Clear_Implementation() override;

#pragma endregion

private:
	int CurrentIndex = -1; // Index of the latest done command
	int LastIndex = -1; // Index of the latest command (CurrentIndex + the number of undone commands) 
	int IndexOffset = 0; // array index wrapping offset

	TArray<TScriptInterface<ICommand>> History = TArray<TScriptInterface<ICommand>>();

	/* Convert from History Index, to Array Index */
	inline int TrueIndex(int Index) { return (Index + IndexOffset) % MaxSize; }

	/* Unvalidated logic for performing undo*/
	void Undo_Impl();

	/* Unvalidated logic for performing redo*/
	void Redo_Impl();

	/* Clear Redoable and Destroy command */
	void ClearRedoable();
};
