// Copyright (c) 2024, Dane Sherman. All rights reserved.
#pragma once

#include "Command.h"
#include "CommandStack.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "CommandHistory.generated.h"

/*
	Use this version of command history if you don't expect frequent use and/or expect very few commands
		- this version dynamically resizes based on the number of commands	

	Commands Enter the history by Push()
	Commands Exit the history by MaxSize check or Clear() -> Destroyed
*/

UCLASS(BlueprintType)
class COMMANDPLUGIN_API UCommandHistory : public UObject, public ICommandStack
{
	GENERATED_BODY()

public:

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

	TArray<TScriptInterface<ICommand>> UndoableHistory = TArray<TScriptInterface<ICommand>>();

	TArray<TScriptInterface<ICommand>> RedoableHistory = TArray<TScriptInterface<ICommand>>();

	/* Unvalidated logic for performing undo*/
	void UndoLatest();

	/* Unvalidated logic for performing redo*/
	void RedoLatest();

	/* Clear Undoable and Destroy command */
	void ClearUndoable();

	/* Clear Redoable and Destroy command */
	void ClearRedoable();
};
