

#include "SaveGameSubsystem.h"
#include <Kismet/GameplayStatics.h>
#include <string>

const FString USaveGameSubsystem::AutoSaveSlotName = "AutoSave";

void USaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	pActiveSaveGame = Cast<UAVSaveGame>(UGameplayStatics::CreateSaveGameObject(UAVSaveGame::StaticClass()));
}

void USaveGameSubsystem::Deinitialize()
{
	if(GetWorld() && AutoSaveTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(AutoSaveTimerHandle);
		AutoSaveTimerHandle.Invalidate();
	}
	RegisteredSavables.Empty();
	Super::Deinitialize();
}

void USaveGameSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	// TODO: Check if its playable map if not, return
	if(bAutoSaveEnable)
	{
		RestartAutoSaveTimer();
	}
}

void USaveGameSubsystem::RegisterSavable(TScriptInterface<IAVSavableInterface> Savable)
{
	if(!Savable)
	{
		// TODO: Log error
		return;
	}
	RegisteredSavables.AddUnique(Savable);
}

void USaveGameSubsystem::UnregisterSavable(TScriptInterface<IAVSavableInterface> Savable)
{
	if(!Savable)
	{
		// TODO: Log error
		return;
	}
	RegisteredSavables.Remove(Savable);
}

void USaveGameSubsystem::StoreStreamingEntity(const FEntitySaveRecord& Record)
{
	if(Record.EntityID == NAME_None)
	{
		// TODO: Log error
		return;
	}
	StreamingHotStore.Add(Record.EntityID, Record);
}

bool USaveGameSubsystem::TryRestoreStreamingEntity(const FName& EntityID, FEntitySaveRecord& OutRecord)
{
	if(FEntitySaveRecord* pRecord = StreamingHotStore.Find(EntityID))
	{
		OutRecord = *pRecord;
		return true;
	}
	// TODO: Log error
	return false;
}

void USaveGameSubsystem::SaveAsync(const FString& SlotName)
{
	if(bSaveInProgress)
	{
		// TODO: Log error
		return;
	}
	if(!IsValid(pActiveSaveGame))
	{
		// TODO: Log error
		return;
	}
	bSaveInProgress = true;
	pActiveSaveGame->SlotName = SlotName;
	pActiveSaveGame->RealWorldSaveTime = FDateTime::Now();
	
	CollectAllEntityData();

	for(const auto& Pair : StreamingHotStore)
	{
		pActiveSaveGame->StreamingEntityData.Add(Pair.Key, Pair.Value);
	}

	FAsyncSaveGameToSlotDelegate SaveDelegate;
	SaveDelegate.BindUObject(this, &USaveGameSubsystem::OnSaveGameComplete);
	UGameplayStatics::AsyncSaveGameToSlot(pActiveSaveGame, SlotName, 0, SaveDelegate);
}

void USaveGameSubsystem::LoadAsync(const FString& SlotName)
{
	if(bLoadInProgress)
	{
		// TODO: Log error Operation already in progress skipping...
		return;
	}
	if(!UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		// TODO: Log error Save game not found: %s
		return;
	}

	bLoadInProgress = true;

	FAsyncLoadGameFromSlotDelegate LoadDelegate;
	LoadDelegate.BindUObject(this, &USaveGameSubsystem::OnLoadGameComplete);
	UGameplayStatics::AsyncLoadGameFromSlot(SlotName, 0, LoadDelegate);
}

void USaveGameSubsystem::SetAutoSaveInterval(float IntervalSeconds)
{
	AutoSaveIntervalSeconds = FMath::Max(60.0f, IntervalSeconds);
	if(bAutoSaveEnable)
	{
		RestartAutoSaveTimer();
	}
}

void USaveGameSubsystem::SetAutoSaveEnabled(bool bEnabled)
{
	bAutoSaveEnable = bEnabled;
	if(bAutoSaveEnable)
	{
		RestartAutoSaveTimer();
	}
	else
	{
		if(GetWorld() && AutoSaveTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(AutoSaveTimerHandle);
			AutoSaveTimerHandle.Invalidate();
		}
	}
}

void USaveGameSubsystem::OnAutoSaveTick()
{
	// TODO: Log auto save triggered
	SaveAsync(AutoSaveSlotName);
}

void USaveGameSubsystem::RestartAutoSaveTimer()
{
	if(!GetWorld() )
	{
		return;
	}
	GetWorld()->GetTimerManager().ClearTimer(AutoSaveTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(AutoSaveTimerHandle, this, &USaveGameSubsystem::OnAutoSaveTick, AutoSaveIntervalSeconds, true);
}

void USaveGameSubsystem::CollectAllEntityData()
{
	if(!IsValid(pActiveSaveGame))
	{
		// TODO: Log error
		return;
	}
	pActiveSaveGame->EntityData.Empty();

	for(TScriptInterface<IAVSavableInterface> Savable : RegisteredSavables)
	{
		if(!Savable)
		{
			continue;
		}

		IAVSavableInterface::Execute_OnPreSave(Savable.GetObject());
		FEntitySaveRecord Record;

		if(IAVSavableInterface::Execute_CollectSaveData(Savable.GetObject(), Record))
		{
			if(Record.EntityID == NAME_None)
			{
				// TODO: Log error
				continue;
			}
			pActiveSaveGame->EntityData.Add(Record.EntityID, Record);
		}
	}
}

void USaveGameSubsystem::DistributeLoadedData()
{
	if(!IsValid(pActiveSaveGame))
	{
		// TODO: Log error
		return;
	}
	StreamingHotStore = pActiveSaveGame->StreamingEntityData;

	for (TScriptInterface<IAVSavableInterface>& Savable : RegisteredSavables)
	{
		if(!Savable)
		{
			continue;
		}

		FName SaveID = IAVSavableInterface::Execute_GetSaveID(Savable.GetObject());
		if(SaveID == NAME_None)
		{
			continue;
		}

		FEntitySaveRecord* pRecord = pActiveSaveGame->EntityData.Find(SaveID);
		if(pRecord)
		{
			IAVSavableInterface::Execute_ApplySaveData(Savable.GetObject(), *pRecord);
		}
		IAVSavableInterface::Execute_OnPostLoad(Savable.GetObject());
	}
}

void USaveGameSubsystem::OnSaveGameComplete(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{	
	bSaveInProgress = false;
	if(bSuccess)
	{
		// TODO: Log success
	}
	else
	{
		// TODO: Log error
	}
	OnSaveComplete.Broadcast(bSuccess);
}

void USaveGameSubsystem::OnLoadGameComplete(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedSaveGame)
{
	bLoadInProgress = false;
	UAVSaveGame* CastedSaveGame = Cast<UAVSaveGame>(LoadedSaveGame);
	if(!IsValid(CastedSaveGame))
	{
		// TODO: Log error
		OnLoadComplete.Broadcast(false);
		return;
	}

	pActiveSaveGame = CastedSaveGame;
	DistributeLoadedData();

	// TODO: Log success
	OnLoadComplete.Broadcast(true);
}
