
#include "AVInventoryComponent.h"
#include "AVItemData.h"

UAVInventoryComponent::UAVInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

bool UAVInventoryComponent::UpdateSlotCount_Implementation(const int NewSlots)
{
	// change number of slots in inventory
	TotalNumberOfSlots = NewSlots;
	ResizeInventory();
	UpdateWeight();
	return true;
}

int UAVInventoryComponent::AddItem_Implementation(const FAVItemData& ItemData, const int Quantity, const float Durability)
{
	int numberToAdd = Quantity;

	// if item is present in inventory
	for (auto& item : InventoryContents) 
	{
		if (!item.IsValid() || ItemData.ItemID != item.ItemID || item.Quantity >= ItemData.ItemMaxStackSize)
		{
			continue;
		}
		int stackSpace = ItemData.ItemMaxStackSize - item.Quantity;
		int toAdd = FMath::Min(stackSpace, numberToAdd);

		item.Quantity += toAdd;

		numberToAdd -= toAdd;
		if (numberToAdd <= 0) 
		{
			UpdateWeight();
			return numberToAdd; // return 0
		}
	}

	// add to empty slot
	for (auto& item : InventoryContents)
	{
		if (item.IsValid()) 
		{
			continue;
		}
		int toAdd = FMath::Min(numberToAdd, ItemData.ItemMaxStackSize);

		item.SetItemData(ItemData.ItemID, toAdd, Durability, ItemData.SingleItemWeight);

		numberToAdd -= toAdd;
		if (numberToAdd <= 0)
		{
			UpdateWeight();
			return numberToAdd;  // return 0
		}
	}

	UpdateWeight();
	return numberToAdd; // return number not added
}

int UAVInventoryComponent::AddItemToSlot_Implementation(const FAVItemData& ItemData, const int Quantity, const float Durability, const int index)
{
	// index invalid
	if (index < 0 || index >= TotalNumberOfSlots)
	{
		return AddItem_Implementation(ItemData, Quantity, Durability);
	}

	// Add to empty slot
	int numberToAdd = Quantity;
	if (!InventoryContents[index].IsValid())
	{
		int toAdd = FMath::Min(numberToAdd, ItemData.ItemMaxStackSize);

		InventoryContents[index].SetItemData(ItemData.ItemID, toAdd, Durability, ItemData.SingleItemWeight);
		
		numberToAdd -= toAdd;
		if (numberToAdd > 0) 
		{
			return AddItem_Implementation(ItemData, numberToAdd, Durability);
		}
		UpdateWeight();
		return numberToAdd;
	}

	// Add to occupied slot but contains same item
	if (InventoryContents[index].ItemID == ItemData.ItemID)
	{
		int stackSpace = ItemData.ItemMaxStackSize - InventoryContents[index].Quantity;
		int toAdd = FMath::Min(stackSpace, numberToAdd);

		InventoryContents[index].SetItemData(ItemData.ItemID, toAdd, Durability, ItemData.SingleItemWeight);

		numberToAdd -= toAdd;
		if (numberToAdd > 0)
		{
			return AddItem_Implementation(ItemData, numberToAdd, Durability);
		}
		UpdateWeight();
		return numberToAdd;  // return 0
	}

	return numberToAdd;		// No items Added
}

bool UAVInventoryComponent::RemoveItem_Implementation(const FName ItemId, const int Quantity)
{
	int numberToRemove = Quantity;
	TArray<int> ItemAtIndex;

	// get index of item i.e needed to removed [in reversed]
	for (int i = InventoryContents.Num() - 1; i >= 0; i++)
	{
		if (InventoryContents[i].ItemID == ItemId)
		{
			ItemAtIndex.Add(i);
			int removedFromStack = FMath::Min(numberToRemove, InventoryContents[i].Quantity);
			numberToRemove -= removedFromStack;
			if (numberToRemove <= 0) 
			{
				break;
			}
		}
	}

	// check entire quatity can be removed
	if (numberToRemove > 0) 
	{
		return false; // failed to remove
	}

	// remove items from backward
	numberToRemove = Quantity;
	for (const auto i : ItemAtIndex)
	{
		int stackSize = InventoryContents[i].Quantity;
		int toRemove = FMath::Min(stackSize, numberToRemove);
		
		InventoryContents[i].Quantity -= toRemove;
		if (!InventoryContents[i].IsValid()) 
		{
			InventoryContents[i].Clear();
		}
		
		numberToRemove -= toRemove;
		if (numberToRemove <= 0)
		{
			break;
		}
	}
	UpdateWeight();
	return true;
}

void UAVInventoryComponent::ResizeInventory()
{
	// changed slots is equal to total slots
	if (InventoryContents.Num() == TotalNumberOfSlots) 
	{
		return;
	}

	// added inventory slots
	if (InventoryContents.Num() < TotalNumberOfSlots) 
	{
		while (InventoryContents.Num() < TotalNumberOfSlots) 
		{
			InventoryContents.Add(FAVInventorySlots());
		}
		return;
	}

	// removed inventory slots
	while (InventoryContents.Num() > TotalNumberOfSlots) 
	{
		FAVInventorySlots DroppedItem = InventoryContents.Pop();
		if (DroppedItem.IsValid()) 
		{
			DropItemFromSlotData(DroppedItem);
		}
	}
}

void UAVInventoryComponent::UpdateWeight()
{
	// Calculate all items weight in inventory
	float tempWeight = 0.0f;
	for (const auto& item : InventoryContents)
	{
		if (!item.IsValid()) 
		{
			continue;
		}
		tempWeight += (item.Quantity * item.ItemWeight);
	}
	CurrentCarryWeight = tempWeight;
}

void UAVInventoryComponent::DropItemFromSlotData(const FAVInventorySlots& SlotData)
{
	// TODO: Call spawn manager -> to spawn dropped items to world
}

float UAVInventoryComponent::GetCarryWeightPercentile_Implementation() const
{
	return CurrentCarryWeight / MaxCarryWeight;
}

bool UAVInventoryComponent::TransferItemToInventory(UAVInventoryComponent* TargetInventory, const FName ItemId, const int Quantity, const int TargetIndex)
{
	if(!IsValid(TargetInventory)) 
	{
		return false;
	}

	int tempIndex = -1;
	// get index of item to transfer
	for (int i = 0;i < InventoryContents.Num(); i++)
	{
		if (InventoryContents[i].ItemID == ItemId)
		{
			tempIndex = i;
			break;
		}
	}
	if (tempIndex == -1)
	{
		return false; // item not found
	}

	// copy inventory slot data to transfer
	const FAVInventorySlots itemToTransfer = InventoryContents[tempIndex];

	// remove item from current inventory
	if (!RemoveItem_Implementation(ItemId, Quantity))
	{
		return false;
	}

	// grab item data to move
	FAVItemData itemDataToMove;
	// TODO: fix once item data manager is implemented

	int numberNotAdded = TargetInventory->AddItemToSlot_Implementation(itemDataToMove, Quantity, itemToTransfer.CurrentDurability, TargetIndex);
	// if some items were not added to target inventory, add them back to current inventory
	if (numberNotAdded > 0)
	{
		AddItemToSlot_Implementation(itemDataToMove, numberNotAdded, itemToTransfer.CurrentDurability, tempIndex);
	}
	return true;
}

void UAVInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	ResizeInventory();
	UpdateWeight();

}
