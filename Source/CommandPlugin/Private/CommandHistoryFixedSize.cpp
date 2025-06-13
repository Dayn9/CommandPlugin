// Copyright (c) 2024, Dane Sherman. All rights reserved.

#include "CommandHistoryFixedSize.h"

#include "CommandUtil.h"

UCommandHistoryFixedSize::UCommandHistoryFixedSize()
{
	History.Init(NULL, MaxSize); // fill array with NULL
}

int UCommandHistoryFixedSize::GetMaxSize_Implementation()
{
	return MaxSize;
}

void UCommandHistoryFixedSize::Push_Implementation(const TScriptInterface<ICommand>& Command)
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
	
	int NextIndex = CurrentIndex + 1;

	// max size check, replace oldest in array
	if (NextIndex == MaxSize)
	{
		CommandUtil::DestroyCommand(History[TrueIndex(NextIndex)]); 
		IndexOffset = TrueIndex(1); 
		NextIndex = MaxSize - 1;
	}

	ClearRedoable(); // clear redoable, so history does not branch

	History[TrueIndex(NextIndex)] = Command;
	LastIndex = CurrentIndex = NextIndex;	
}

#pragma region UNDO 

bool UCommandHistoryFixedSize::CanUndo_Implementation()
{
	return CurrentIndex > -1;
}

void UCommandHistoryFixedSize::Undo_Implementation()
{
	if (CanUndo_Implementation()) 
	{
		Undo_Impl();
	}
}

void UCommandHistoryFixedSize::UndoNum_Implementation(int num)
{
	num = FMath::Clamp(num, 0, CurrentIndex + 1);
	for (size_t i = 0; i < num; ++i) 
	{
		Undo_Impl();
	}
}

void UCommandHistoryFixedSize::UndoAll_Implementation()
{
	UndoNum_Implementation(CurrentIndex + 1);
}

void UCommandHistoryFixedSize::Undo_Impl()
{
	TScriptInterface<ICommand> Command = History[TrueIndex(CurrentIndex)];
	ICommand::Execute_Undo(Command.GetObject());
	--CurrentIndex;
}

#pragma endregion

#pragma region REDO

bool UCommandHistoryFixedSize::CanRedo_Implementation()
{
	return CurrentIndex != LastIndex;
}

void UCommandHistoryFixedSize::Redo_Implementation()
{
	if (CanRedo_Implementation()) 
	{
		Redo_Impl();
	}
}

void UCommandHistoryFixedSize::RedoNum_Implementation(int num)
{
	num = FMath::Clamp(num, 0, LastIndex - CurrentIndex);
	for (size_t i = 0; i < num; ++i)
	{
		Redo_Impl();
	}
}

void UCommandHistoryFixedSize::RedoAll_Implementation()
{
	RedoNum_Implementation(LastIndex - CurrentIndex);
}

void UCommandHistoryFixedSize::Redo_Impl()
{
	++CurrentIndex;
	TScriptInterface<ICommand> Command = History[TrueIndex(CurrentIndex)];
	ICommand::Execute_Do(Command.GetObject());
}

#pragma endregion

#pragma region Clear

void UCommandHistoryFixedSize::Clear_Implementation()
{
	// Clear history without re-allocating array
	for (size_t i = LastIndex; i > 0; --i) 
	{
		CommandUtil::DestroyCommand(History[TrueIndex(i)]);
	}

	History.Init(NULL, MaxSize);
	LastIndex = CurrentIndex = -1;
	IndexOffset = 0;
}

void UCommandHistoryFixedSize::ClearRedoable()
{
	if (LastIndex == CurrentIndex)
	{
		return;
	}

	for (size_t i = LastIndex; i > CurrentIndex; --i) 
	{
		CommandUtil::DestroyCommand(History[TrueIndex(i)]);
		History[TrueIndex(i)] = NULL;
	}

	LastIndex = CurrentIndex;
}

#pragma endregion