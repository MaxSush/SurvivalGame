

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ComponentInterfaces/public/AVSavableInterface.h"

#include "AVSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SAVESYSTEM_API UAVSaveGame : public USaveGame, public IAVSavableInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(SaveGame)
	TMap<FName, FEntitySaveRecord> EntityData;

	UPROPERTY(SaveGame)
	TMap<FName, FEntitySaveRecord> StreamingEntityData;

	UPROPERTY(SaveGame)
	FString SlotName;

	UPROPERTY(SaveGame)
	FDateTime RealWorldSaveTime;

	UPROPERTY(SaveGame)
	int32 SaveVersion = 1;

};
