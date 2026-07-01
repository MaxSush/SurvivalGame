

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AVSavableInterface.generated.h"

USTRUCT(BlueprintType)
struct FSaveComponentData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	TArray<uint8> ByteData;

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	UClass* ComponentClass = nullptr;

	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	TArray<FString> RawData;
};

USTRUCT(BlueprintType)
struct FEntitySaveRecord
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(SaveGame)
	FName EntityID = NAME_None;

	UPROPERTY(SaveGame)
	FTransform ActorTransform = FTransform();

	UPROPERTY(SaveGame)
	TArray<uint8> CustomData;

	UPROPERTY(SaveGame)
	TArray<FSaveComponentData> ComponentData;

	UPROPERTY(SaveGame)
	TArray<FString> RawData;

	UPROPERTY(SaveGame)
	bool bWasRuntimeSpawned = false;

	UPROPERTY(SaveGame)
	UClass* ActorClass = nullptr;
};



// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAVSavableInterface : public UInterface
{
	GENERATED_BODY()
};


/*-- - GAME ARCHITECTURE MEMO : THE EXPLICIT SAVE SYSTEM-- -
*
*CORE PRINCIPLE :
*Do NOT modify 'FEntitySaveRecord' or 'FSaveComponentData' when adding new variables.
* Instead, use 'CustomData' (Actors) and 'ByteData' (Components)as raw binary containers.
*
*THE RULE OF SERIALIZATION ORDER :
*Variables MUST be read out in the EXACT same order they were written in.
*
*1. SAVING(PACK DATA) :
	*bool CollectSaveData_Implementation(FEntitySaveRecord & OutRecord) const {
	*OutRecord.EntityID = GetSaveID();
	*OutRecord.ActorTransform = GetActorTransform();
	*
		*FMemoryWriter Writer(OutRecord.CustomData); // Feeds raw byte array
	*Writer << Health << Ammo << Score;          // Bitwise stream execution
	*return true;
	*
}
*
*2. LOADING(UNPACK DATA) :
	*void ApplySaveData_Implementation(const FEntitySaveRecord & Record) {
	*SetActorTransform(Record.ActorTransform); // Restores physics state
	*
		*if (Record.CustomData.Num() > 0) {
		*FMemoryReader Reader(Record.CustomData);
		*Reader << Health << Ammo << Score;    // MUST MATCH WRITER ORDER PERFECTLY!
		*
	}
	*
}
*/


class COMPONENTINTERFACES_API IAVSavableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	FName GetSaveID() const;
	virtual FName GetSaveID_Implementation() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	bool CollectSaveData(FEntitySaveRecord& OutRecord) const;
	virtual bool CollectSaveData_Implementation(FEntitySaveRecord& OutRecord) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	void ApplySaveData(const FEntitySaveRecord& Record);
	virtual void ApplySaveData_Implementation(const FEntitySaveRecord& Record);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	void OnPreSave();
	virtual void OnPreSave_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Save")
	void OnPostLoad();
	virtual void OnPostLoad_Implementation();

};
