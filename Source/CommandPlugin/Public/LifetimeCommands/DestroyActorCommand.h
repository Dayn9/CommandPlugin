#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Command.h"
#include "DestroyActorCommand.generated.h"

UCLASS(BlueprintType, Blueprintable)
class COMMANDPLUGIN_API UDestroyActorCommand : public UObject, public ICommand
{
	GENERATED_BODY()

private:
	bool InLimbo = false;

public:

	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = true))
	TObjectPtr<AActor> ActorToDestroy = nullptr;

	void Do_Implementation() override;
	void Undo_Implementation() override;
	FString GetDisplayString_Implementation() override;

	virtual void BeginDestroy() override;
};
