

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AVInventorySlots.h"
#include "ComponentInterfaces/public/AVInventoryInterface.h"
#include "AVInventoryComponent.generated.h"


struct FAVItemData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORY_API UAVInventoryComponent : public UActorComponent, public IAVInventoryInterface
{
	GENERATED_BODY()

public:	
	UAVInventoryComponent();

	// IAVInventoryInterface methods
	virtual bool UpdateSlotCount_Implementation(const int NewSlots) override;
	virtual int AddItem_Implementation(const FAVItemData& ItemData, const int Quantity, const float Durability = -1.0f) override;
	virtual int AddItemToSlot_Implementation(const FAVItemData& ItemData, const int Quantity, const float Durability = -1.0f, const int index = -1) override;
	virtual bool RemoveItem_Implementation(const FName ItemId, const int Quantity) override;
	virtual float GetCarryWeightPercentile_Implementation() const override;

	bool TransferItemToInventory(UAVInventoryComponent* TargetInventory, const FName ItemId, const int Quantity, const int TargetIndex = -1);

protected:
	virtual void BeginPlay() override;
	virtual void ResizeInventory();
	virtual void UpdateWeight();


private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int TotalNumberOfSlots = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FAVInventorySlots> InventoryContents;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float MaxCarryWeight = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CurrentCarryWeight = 0.0f;


	void DropItemFromSlotData(const FAVInventorySlots& SlotData);
};
