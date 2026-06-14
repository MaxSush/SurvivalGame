
#include "AVCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AVInventoryComponent.h"


AAVCharacterBase::AAVCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	InventoryComponent = CreateDefaultSubobject<UAVInventoryComponent>(TEXT("InventoryComponent"));

}

void AAVCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AAVCharacterBase::CanCharacterJump() const
{
	return CanJump();
}

void AAVCharacterBase::HasJumped()
{
	ACharacter::Jump();
}

float AAVCharacterBase::GetSneakSpeed() const
{
	return SneakSpeed;
}

float AAVCharacterBase::GetWalkSpeed() const
{
	return WalkSpeed;
}

float AAVCharacterBase::GetSprintSpeed() const
{
	return SprintSpeed;
}

void AAVCharacterBase::SetSprinting(const bool bSprinting)
{
	if (bSprinting) 
	{
		bIsSneaking = false;
		GetCharacterMovement()->MaxWalkSpeed = GetSprintSpeed();
		bIsSprinting = true;
		return;
	}
	if (bIsSneaking) 
	{
		return;
	}
	GetCharacterMovement()->MaxWalkSpeed = GetWalkSpeed();
	bIsSprinting = false;
	return;
}

void AAVCharacterBase::SetSneaking(const bool bSneaking)
{
	if (bSneaking) 
	{
		bIsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = GetSneakSpeed();
		bIsSneaking = true;
		return;
	}
	if (bIsSprinting) 
	{
		return;
	}
	GetCharacterMovement()->MaxWalkSpeed = GetWalkSpeed();
	bIsSneaking = false;
	return;
}

void AAVCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAVCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UActorComponent* AAVCharacterBase::GetInventoryComponent() const
{
	return Cast<UActorComponent>(InventoryComponent);
}


