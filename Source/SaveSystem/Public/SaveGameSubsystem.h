
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ComponentInterfaces/public/AVSavableInterface.h"
#include "AVSaveGame.h"
#include "SaveGameSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveCompleteDelegate, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadCompleteDelegate, bool, bSuccess);


UCLASS()
class SAVESYSTEM_API USaveGameSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	void RegisterSavable(TScriptInterface<IAVSavableInterface> Savable);
	void UnregisterSavable(TScriptInterface<IAVSavableInterface> Savable);

	void StoreStreamingEntity(const FEntitySaveRecord& Record);
	bool TryRestoreStreamingEntity(const FName& EntityID, FEntitySaveRecord& OutRecord);

	UFUNCTION(BlueprintCallable, Category = "Save System")
	void SaveAsync(const FString& SlotName);
	UFUNCTION(BlueprintCallable, Category = "Save System")
	void LoadAsync(const FString& SlotName);


	UPROPERTY(BlueprintAssignable, Category = "Save System")
	FOnSaveCompleteDelegate OnSaveComplete;
	UPROPERTY(BlueprintAssignable, Category = "Save System")
	FOnLoadCompleteDelegate OnLoadComplete;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AutoSaveIntervalSeconds = 900.0f; // 15 min
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bAutoSaveEnable = true;

	UFUNCTION(BlueprintCallable, Category = "Auto Save")
	void SetAutoSaveInterval(float IntervalSeconds);

	UFUNCTION(BlueprintCallable, Category = "Auto Save")
	void SetAutoSaveEnabled(bool bEnabled);

private:
	FTimerHandle AutoSaveTimerHandle;
	static const FString AutoSaveSlotName;	// init "AutoSave"
	void OnAutoSaveTick();
	void RestartAutoSaveTimer();

	TArray<TScriptInterface<IAVSavableInterface>> RegisteredSavables;
	TMap<FName, FEntitySaveRecord> StreamingHotStore;

	UPROPERTY()
	UAVSaveGame* pActiveSaveGame = nullptr;

	bool bSaveInProgress = false;
	bool bLoadInProgress = false;

	void CollectAllEntityData();
	void DistributeLoadedData();

	UFUNCTION()
	void OnSaveGameComplete(const FString& SlotName, const int32 UserIndex, bool bSuccess);
	UFUNCTION()
	void OnLoadGameComplete(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedSaveGame);
};
