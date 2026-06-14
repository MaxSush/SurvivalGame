

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AVInventoryInterface.generated.h"


struct FAVItemData;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAVInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

class COMPONENTINTERFACES_API IAVInventoryInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool UpdateSlotCount(const int NewSlots);
	virtual bool UpdateSlotCount_Implementation(const int NewSlots);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int AddItem(const FAVItemData& ItemData, const int Quantity, const float Durability = -1.0f);
	virtual int AddItem_Implementation(const FAVItemData& ItemData, const int Quantity, const float Durability = -1.0f);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int AddItemToSlot(const FAVItemData& ItemData, const int Quantity, const float Durability = -1.0f, const int index = -1);
	virtual int AddItemToSlot_Implementation(const FAVItemData& ItemData, const int Quantity, const float Durability = -1.0f, const int index = -1);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool RemoveItem(const FName ItemId, const int Quantity);
	virtual bool RemoveItem_Implementation(const FName ItemId, const int Quantity);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetCarryWeightPercentile() const;
	virtual float GetCarryWeightPercentile_Implementation() const;
};
