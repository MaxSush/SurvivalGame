

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AVCharacterBase.generated.h"

UCLASS(Abstract, NotBlueprintable)
class CHARACTERS_API AAVCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:	
	AAVCharacterBase();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	bool CanCharacterJump() const;
	void HasJumped();

	float GetSneakSpeed() const;
	float GetWalkSpeed() const;
	float GetSprintSpeed() const;

	void SetSprinting(const bool bSprinting);
	void SetSneaking(const bool bSneaking);


private:
#pragma region Movement

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsSprinting = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsSneaking = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float SneakSpeed = 150.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed = 300.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float SprintSpeed = 600.f;

#pragma endregion
};
