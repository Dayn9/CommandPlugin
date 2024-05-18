#include "CommandHistoryExperimental.h"
#include <CommandUtil.h>

UCommandHistoryExperimental::UCommandHistoryExperimental()
{
	History.Init(NULL, MaxSize); // fill array with NULL
}

int UCommandHistoryExperimental::GetMaxSize_Implementation()
{
	return MaxSize;
}

void UCommandHistoryExperimental::Push_Implementation(const TScriptInterface<ICommand>& Command)
{
	/* perform null checks here, this is the only entry point for commands into the history */
	if (&Command == NULL)
		return;

	UObject* CommandObject = Command.GetObject();
	if (CommandObject == NULL)
		return;

	if (ICommand::Execute_Do(CommandObject))
	{
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
}

#pragma region UNDO 

bool UCommandHistoryExperimental::CanUndo_Implementation()
{
	return CurrentIndex > -1;
}

void UCommandHistoryExperimental::Undo_Implementation()
{
	if (CanUndo())
		Undo_Impl();
}

void UCommandHistoryExperimental::UndoNum_Implementation(int num)
{
	num = FMath::Clamp(num, 0, CurrentIndex + 1);
	for (size_t i = 0; i < num; ++i)
		Undo_Impl();
}

void UCommandHistoryExperimental::UndoAll_Implementation()
{
	UndoNum(CurrentIndex + 1);
}

void UCommandHistoryExperimental::Undo_Impl()
{
	TScriptInterface<ICommand> Command = History[TrueIndex(CurrentIndex)];
	ICommand::Execute_Undo(Command.GetObject());
	--CurrentIndex;
}

#pragma endregion

#pragma region REDO

bool UCommandHistoryExperimental::CanRedo_Implementation()
{
	return CurrentIndex != LastIndex;
}

void UCommandHistoryExperimental::Redo_Implementation()
{
	if (CanRedo())
		Redo_Impl();
}

void UCommandHistoryExperimental::RedoNum_Implementation(int num)
{
	num = FMath::Clamp(num, 0, LastIndex - CurrentIndex);
	for (size_t i = 0; i < num; ++i)
		Redo_Impl();
}

void UCommandHistoryExperimental::RedoAll_Implementation()
{
	RedoNum(LastIndex - CurrentIndex);
}

void UCommandHistoryExperimental::Redo_Impl()
{
	++CurrentIndex;
	TScriptInterface<ICommand> Command = History[TrueIndex(CurrentIndex)];
	ICommand::Execute_Do(Command.GetObject());
}

#pragma endregion

#pragma region Clear

void UCommandHistoryExperimental::Clear_Implementation()
{
	// Clear history without re-allocating array
	for (size_t i = LastIndex; i > 0; --i) 
		CommandUtil::DestroyCommand(History[TrueIndex(i)]);

	History.Init(NULL, MaxSize);
	LastIndex = CurrentIndex = -1;
	IndexOffset = 0;
}

void UCommandHistoryExperimental::ClearRedoable()
{
	if (LastIndex == CurrentIndex) return;

	for (size_t i = LastIndex; i > CurrentIndex; --i) 
	{
		CommandUtil::DestroyCommand(History[TrueIndex(i)]);
		History[TrueIndex(i)] = NULL;
	}
	LastIndex = CurrentIndex;
}

#pragma endregion