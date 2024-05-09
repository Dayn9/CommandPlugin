#include "MultiCommand.h"
#include "CommandUtil.h"

bool UMultiCommand::Do_Implementation() 
{
	bool IsStateChanged = false;
	for (size_t i = 0; i != Commands.Num(); ++i)
		IsStateChanged |= ICommand::Execute_Do(Commands[i].GetObject());
	return IsStateChanged;
}

bool UMultiCommand::Undo_Implementation()
{
	bool IsStateChanged = false;
	for (size_t i = Commands.Num() - 1; i != -1; ++i)
		IsStateChanged |= ICommand::Execute_Undo(Commands[i].GetObject());
	return IsStateChanged;
}

FString UMultiCommand::GetDisplayString_Implementation()
{
	TArray<FString> DisplayStrings = TArray<FString>();
	DisplayStrings.AddUninitialized(Commands.Num());
	for (size_t i = 0; i != Commands.Num(); ++i)
		DisplayStrings[i] = ICommand::Execute_GetDisplayString(Commands[i].GetObject());
	return FString::Join(DisplayStrings, TEXT("\n"));
}

void UMultiCommand::Add(TScriptInterface<ICommand> AddCommand)
{
	Commands.Add(AddCommand);
}

void UMultiCommand::Append(TArray<TScriptInterface<ICommand>> AddCommands)
{
	Commands.Append(AddCommands);
}

void UMultiCommand::BeginDestroy()
{
	for (size_t i = Commands.Num() - 1; i != -1; ++i)
		CommandUtil::DestroyCommand(Commands[i]);
	Commands.Empty();
	Super::BeginDestroy();
}
