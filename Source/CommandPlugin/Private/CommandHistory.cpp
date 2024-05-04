#include "CommandHistory.h"

void UCommandHistory::Push(TScriptInterface<ICommand> Command)
{
	/* perform null checks here, this is the only entry point for commands into the history */
	if (&Command == NULL) return; 

	UObject* CommandObject = Command.GetObject();
	if (CommandObject == NULL) return;

	ICommand::Execute_Do(CommandObject);
	UndoableHistory.Push(Command);

	ClearRedoable(); // no branching history
}

bool UCommandHistory::CanUndo()
{
	return UndoableHistory.Num() > 0;
}

void UCommandHistory::Undo()
{
	if (!CanUndo()) return;

	// pop -> push
	TScriptInterface<ICommand> Command = UndoableHistory.Pop(true); 
	ICommand::Execute_Undo(Command.GetObject());
	RedoableHistory.Push(Command);
}

void UCommandHistory::UndoNum(int num)
{
}

void UCommandHistory::UndoAll()
{
	if (!CanUndo()) return;

	for (size_t i = UndoableHistory.Num() - 1; i != -1; --i)
		ICommand::Execute_Undo(UndoableHistory[i].GetObject());

	Algo::Reverse(UndoableHistory);
	RedoableHistory.Append(UndoableHistory);
	UndoableHistory.Empty();

}

bool UCommandHistory::CanRedo()
{
	return RedoableHistory.Num() > 0;
}

void UCommandHistory::Redo()
{
	if (!CanRedo()) return;

	// pop -> push
	TScriptInterface<ICommand> Command = RedoableHistory.Pop(true);
	ICommand::Execute_Do(Command.GetObject());
	UndoableHistory.Push(Command);
}

void UCommandHistory::RedoNum(int num)
{
}

void UCommandHistory::RedoAll()
{
	if (!CanRedo()) return;

	for (size_t i = RedoableHistory.Num() - 1; i != -1; --i)
		ICommand::Execute_Undo(RedoableHistory[i].GetObject());

	Algo::Reverse(RedoableHistory);
	UndoableHistory.Append(RedoableHistory);
	RedoableHistory.Empty();
}

void UCommandHistory::Clear()
{
	ClearUndoable();
	ClearRedoable();
}

void UCommandHistory::ClearUndoable()
{
	for (auto& Command : UndoableHistory)
		ClearCommand(Command);

	UndoableHistory.Empty();
}

void UCommandHistory::ClearRedoable()
{
	for (auto& Command : RedoableHistory)
		ClearCommand(Command);

	RedoableHistory.Empty();
}

void UCommandHistory::ClearCommand(TScriptInterface<ICommand> Command)
{
	UObject* CommandObject = Command.GetObject();
	if (CommandObject->IsValidLowLevel()) {
		CommandObject->ConditionalBeginDestroy();
	}	
}
