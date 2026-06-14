
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AVItemData.generated.h"

USTRUCT(BlueprintType)
struct FAVItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemMaxStackSize = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SingleItemWeight = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDurabilty = -1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseStaticMesh = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemName = FText::GetEmpty();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemDescription = FText::GetEmpty();

	FORCEINLINE bool IsValid() const
	{
		return ItemID != NAME_None && ItemMaxStackSize > 0;
	}

	FORCEINLINE void Clear()
	{
		ItemID = NAME_None;
		ItemMaxStackSize = 0;
		SingleItemWeight = 1.0f;
		MaxDurabilty = 0.0f;
		Icon = nullptr;
		Mesh = nullptr;
		SkeletalMesh = nullptr;
		bUseStaticMesh = true;
		ItemName = FText::GetEmpty();
		ItemDescription = FText::GetEmpty();
	}
};
