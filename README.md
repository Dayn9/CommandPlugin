# CommandPlugin
Command Pattern for implementing undo/redo systems in Unreal Engine. 

### Command Pattern? 

The basic idea behind using the command pattern for undo/redo is to wrap any function that needs to be undoable in a command. 
Inside that command, we can define what happens when the command is executed (done) and what happens when the command is unexecuted (undo).
We can then store a list of these commands in a history stack that allows us to step backward and forward through the commands when undoing and redoing.

For more information on the command pattern, see: [refactoring guru - command pattern](https://refactoring.guru/design-patterns/command)

> Note: This Unreal Engine implementation uses "Do" instead of "Execute".
This change was made to avoid confusion when calling [UINTERFACE](https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/GameplayArchitecture/Interfaces/) functions in C++, which use a generated Execute_ function: ICommand::Execute_Do();

### Guidelines

Hard-ish rules to be aware of. Be careful if your use case needs to break one. 

1) Commands should be used for actions that change the state of the application. If a user interaction does not change the state (moving the cursor for example), it should not be tracked. 

2) Every ICommand that implements `ICommand.Do()` should also implement `ICommand.Undo()` where: 
   - `ICommand.Do()` sets some state to a new value
   - `ICommand.Undo()` restores that state to the original value

3) ICommand Do and Undo should only ever be called from within the controlling ICommandStack. 
`ICommand.Do()` should only ever be called in the `ICommandStack.Push()` function when first executed or the `ICommandStack.Redo()` function when redone.
Similarly, `ICommand.Undo()` should only ever be called in the `ICommandStack.Undo()` function when undone.

4) `ICommand.GetDisplayString()` is **not required** but if used, should reflect the new value set by executing the command. return a user-friendly message. 
This message should reflect the new value, which is set by calling `ICommand.Do()`

### Function -> ICommand

Here is an example of how to turn a function into a command:
Let's start with an example object `ExampleState` with a variable `X` that we want to track in the command history.

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

To track changes to `X` in a command, we will need:
1) a reference to the object it's being called on
3) the new value 

These will be assigned in the constructor of our command (this would be exposed on spawn in blueprint)

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

	void Do_Implementation() override
	{
		Target->SetX(NewX);
	}

	void Undo_Implementation() override
	{
		Target->SetX(OldX);
	}

	FString GetDisplayString_Implementation() override
	{
		return "updated X on target";
	} 
}
```

Notice that `OldX` does not need to be passed into the constructor. By holding a reference to the state object, we can instead call `GetX()` to cache the current state. 
