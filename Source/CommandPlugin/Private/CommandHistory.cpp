#include "CommandHistory.h"

void UCommandHistory::Push(TScriptInterface<ICommand> Command)
{
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
	if (&Command == NULL) return;

	UObject* CommandObject = Command.GetObject();
	if (!CommandObject->IsValidLowLevel()) return;

	CommandObject->ConditionalBeginDestroy();
}
