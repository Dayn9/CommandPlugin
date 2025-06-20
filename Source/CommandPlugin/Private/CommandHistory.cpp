// Copyright (c) 2024, Dane Sherman. All rights reserved.

#include "CommandHistory.h"

#include "CommandUtil.h"

int UCommandHistory::GetMaxSize_Implementation()
{
	return MaxSize;
}

void UCommandHistory::Push_Implementation(const TScriptInterface<ICommand>& Command)
{
	/* perform null checks here, this is the only entry point for commands into the history */
	if (&Command == NULL) 
	{
		return; 
	}

	UObject* CommandObject = Command.GetObject();
	if (CommandObject == NULL) 
	{
		return;
	}

	ICommand::Execute_Do(CommandObject);

	if (UndoableHistory.Num() == MaxSize)
	{
		CommandUtil::DestroyCommand(UndoableHistory[0]); // when max size, destroy 'oldest'
		UndoableHistory.RemoveAt(0);
	}

	UndoableHistory.Push(Command);

	ClearRedoable(); // no branching history
}

#pragma region UNDO 

bool UCommandHistory::CanUndo_Implementation()
{
	return UndoableHistory.Num() > 0;
}

void UCommandHistory::Undo_Implementation()
{
	if (CanUndo_Implementation()) 
	{
		UndoLatest();
	}
}

void UCommandHistory::UndoNum_Implementation(int num)
{
	num = FMath::Clamp(num, 0, UndoableHistory.Num());
	for (size_t i = 0; i < num; ++i) 
	{
		UndoLatest();
	}
}

void UCommandHistory::UndoAll_Implementation()
{
	if (CanUndo_Implementation())
	{
		for (size_t i = UndoableHistory.Num() - 1; i != -1; --i) 
		{
			ICommand::Execute_Undo(UndoableHistory[i].GetObject());
		}

		Algo::Reverse(UndoableHistory);
		RedoableHistory.Append(UndoableHistory);
		UndoableHistory.Empty();
	}
}

void UCommandHistory::UndoLatest()
{
	TScriptInterface<ICommand> Command = UndoableHistory.Pop(true);
	ICommand::Execute_Undo(Command.GetObject());
	RedoableHistory.Push(Command);
}

#pragma endregion

#pragma region REDO

bool UCommandHistory::CanRedo_Implementation()
{
	return RedoableHistory.Num() > 0;
}

void UCommandHistory::Redo_Implementation()
{
	if (CanRedo_Implementation()) 
	{
		RedoLatest();
	}
}

void UCommandHistory::RedoNum_Implementation(int num)
{
	num = FMath::Clamp(num, 0, RedoableHistory.Num());
	for (size_t i = 0; i < num; ++i) 
	{
		RedoLatest();
	}
}

void UCommandHistory::RedoAll_Implementation()
{
	if (CanRedo_Implementation()) 
	{
		for (size_t i = RedoableHistory.Num() - 1; i != -1; --i) 
		{
			ICommand::Execute_Undo(RedoableHistory[i].GetObject());
		}

		Algo::Reverse(RedoableHistory);
		UndoableHistory.Append(RedoableHistory);
		RedoableHistory.Empty();
	}
}

void UCommandHistory::RedoLatest()
{
	TScriptInterface<ICommand> Command = RedoableHistory.Pop(true);
	ICommand::Execute_Do(Command.GetObject());
	UndoableHistory.Push(Command);
}

#pragma endregion

#pragma region Clear

void UCommandHistory::Clear_Implementation()
{
	ClearUndoable();
	ClearRedoable();
}

void UCommandHistory::ClearUndoable()
{
	for (auto& Command : UndoableHistory) 
	{
		CommandUtil::DestroyCommand(Command);
	}

	UndoableHistory.Empty();
}

void UCommandHistory::ClearRedoable()
{
	for (auto& Command : RedoableHistory) 
	{
		CommandUtil::DestroyCommand(Command);
	}

	RedoableHistory.Empty();
}

#pragma endregion