


#include "AVSavableInterface.h"


// Add default functionality here for any IAVSavableInterface functions that are not pure virtual.

FName IAVSavableInterface::GetSaveID_Implementation() const
{
	return FName();
}

bool IAVSavableInterface::CollectSaveData_Implementation(FEntitySaveRecord& OutRecord) const
{
	return false;
}

void IAVSavableInterface::ApplySaveData_Implementation(const FEntitySaveRecord& Record)
{
}

void IAVSavableInterface::OnPreSave_Implementation()
{
}

void IAVSavableInterface::OnPostLoad_Implementation()
{
}
