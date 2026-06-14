#pragma once

#include "CoreMinimal.h"
#include "AVInventorySlots.generated.h"


USTRUCT(BlueprintType)
struct FAVInventorySlots
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName ItemID = NAME_None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int Quantity = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentDurability = -1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float ItemWeight = 0.0f;

	FAVInventorySlots() = default;

	FORCEINLINE void SetItemData(const FName ItemId, const int ItemQuantity, const float Durability, const float Weight)
	{
		this->ItemID = ItemId;
		this->Quantity = Quantity;
		this->CurrentDurability = Durability;
		this->ItemWeight = Weight;
	}

	FORCEINLINE void Clear()
	{
		ItemID = NAME_None;
		Quantity = 0;
		CurrentDurability = -1.0f;
		ItemWeight = 0.0f;
	}

	FORCEINLINE bool IsValid() const
	{
		return ItemID != NAME_None && Quantity > 0;
	}

};