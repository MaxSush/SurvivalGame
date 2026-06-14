


#include "AVInventoryInterface.h"
#include "AVItemData.h"


// Add default functionality here for any IAVInventoryInterface functions that are not pure virtual.

bool IAVInventoryInterface::UpdateSlotCount_Implementation(const int NewSlots)
{
	return false;
}

int IAVInventoryInterface::AddItem_Implementation(const FAVItemData& ItemData, const int Quantity, const float Durability)
{
	return Quantity;
}

int IAVInventoryInterface::AddItemToSlot_Implementation(const FAVItemData& ItemData, const int Quantity, const float Durability, const int index)
{
	return Quantity;
}

bool IAVInventoryInterface::RemoveItem_Implementation(const FName ItemId, const int Quantity)
{
	return false;
}

float IAVInventoryInterface::GetCarryWeightPercentile_Implementation() const
{
	return -1.0f;
}
