#include "CommandHistoryExperimental.h"
#include <CommandUtil.h>

void UCommandHistoryExperimental::Push(TScriptInterface<ICommand> Command)
{
	/* perform null checks here, this is the only entry point for commands into the history */
	if (&Command == NULL)
		return;

	UObject* CommandObject = Command.GetObject();
	if (CommandObject == NULL)
		return;

	if (ICommand::Execute_Do(CommandObject))
	{
		/*
		if (History.Num() == MaxSize)
		{
			CommandUtil::DestroyCommand(History[0]); // when max size, destroy 'oldest'
		}
		*/
		ClearRedoable(); // no branching history

		CurrentIndex = History.Add(Command);
		LastIndex = CurrentIndex;
	}
}

#pragma region UNDO 

bool UCommandHistoryExperimental::CanUndo()
{
	return CurrentIndex > -1;
}

void UCommandHistoryExperimental::Undo()
{
	if (CanUndo())
		UndoLatest();
}

void UCommandHistoryExperimental::UndoNum(int num)
{
	num = FMath::Clamp(num, 0, CurrentIndex + 1);
	for (size_t i = 0; i < num; ++i)
		UndoLatest();
}

void UCommandHistoryExperimental::UndoAll()
{
	UndoNum(CurrentIndex + 1);
}

void UCommandHistoryExperimental::UndoLatest()
{
	TScriptInterface<ICommand> Command = History[CurrentIndex];
	ICommand::Execute_Undo(Command.GetObject());
	--CurrentIndex;
}

#pragma endregion

#pragma region REDO

bool UCommandHistoryExperimental::CanRedo()
{
	return CurrentIndex != LastIndex;
}

void UCommandHistoryExperimental::Redo()
{
	if (CanRedo())
		RedoLatest();
}

void UCommandHistoryExperimental::RedoNum(int num)
{
	num = FMath::Clamp(num, 0, LastIndex - CurrentIndex);
	for (size_t i = 0; i < num; ++i)
		RedoLatest();
}

void UCommandHistoryExperimental::RedoAll()
{
	RedoNum(LastIndex - CurrentIndex);
}

void UCommandHistoryExperimental::RedoLatest()
{
	++CurrentIndex;
	TScriptInterface<ICommand> Command = History[CurrentIndex];
	ICommand::Execute_Do(Command.GetObject());
}

#pragma endregion

#pragma region Clear

void UCommandHistoryExperimental::Clear()
{
	// Clear history without re-allocating array
	for (size_t i = LastIndex; i > 0; --i) 
		CommandUtil::DestroyCommand(History[i]);
	History.RemoveAt(0, LastIndex + 1, false);

	CurrentIndex = 0;
	LastIndex = 0;
}

void UCommandHistoryExperimental::ClearRedoable()
{
	if (LastIndex == CurrentIndex) return;

	for (size_t i = LastIndex; i > CurrentIndex; --i) 
		CommandUtil::DestroyCommand(History[i]);
	History.RemoveAt(CurrentIndex + 1, LastIndex - CurrentIndex, false);
	LastIndex = CurrentIndex;
}

#pragma endregion