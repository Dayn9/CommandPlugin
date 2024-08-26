# CommandPlugin
Command Pattern for implementing undo/redo systems in Unreal Engine. 


### Command Pattern? 

The basic idea behind using the command pattern for undo / redo is to wrap any function that needs to be undoable with a command. 
Inside that command we can define what happens when the command is executed / done and what happens when the command is unexecuted or undone.
We can then store a list of these commands in some sort of history that allows us to step backwards and forwards through the commands when undoing and redoing.

For more information on the command pattern see: [refactoring guru - command pattern](https://refactoring.guru/design-patterns/command)

Note: This Unreal Engine implementation uses "Do" instead of "Execute".
This change was made to avoid confusion when calling [UINTERFACE](https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/GameplayArchitecture/Interfaces/) functions in C++, which use a generated Execute_ function: ICommand::Execute_Do();


### Guidelines

Hard-ish rules to be aware of. Be careful if your use case needs to break one. 

1) Commands should be used for actions that change the state of the application. 

2) Every ICommand should implement `ICommand.Do()` AND `ICommand.Undo()` where: 
   - `ICommand.Do()` sets some state to a new value
   - `ICommand.Undo()` restores that state to the original value

3) `ICommand.Do()` should only ever be called in the `ICommandStack.Push()` function or the `ICommandStack.Redo()` functions.
Similarly, `ICommand.Undo()` should only ever be called in the `ICommandStack.Undo()` functions.

4) `ICommand.GetDisplayString()` is **not required**. 
But if used, should return a user freindly message. 
This message should reflect the new value, which is set by calling `ICommand.Do()`

### Function -> ICommand

Here is an example of how to turn a function into a command:

```
class ExampleState
{
private: 
	float X;
public: 
	float GetX() { return X; }
	void SetX(float NewX) { X = NewX; }
}
```

In order to call `SetX()` we will need:
1) a reference to the object it's being called on
2) the float value we want to use

These will be assigned in the costructor (or exposed on spawn in blueprint)

```
class ExampleCommand : pubic ICommand
{
	ExampleState* Target;
	float NewX;
	float OldX;

	ExampleCommand(ExampleState* InTarget, float InNewX)
	{
		Target = InTarget;
		NewX = InNewX;
		OldX = Target->GetX(); // store the old value 
	}

	void Do_Implementation() override { Target->SetX(NewValue); }
	void Undo_Implementation() override { Target->SetX(OldValue); }
	FString GetDisplayString_Implementation() overrid { return "SetX"; } 
}
```

