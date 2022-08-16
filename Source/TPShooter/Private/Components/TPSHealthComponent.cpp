// Third Player Shooter, All Rights Reserved


#include "Components/TPSHealthComponent.h"

UTPSHealthComponent::UTPSHealthComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts
void UTPSHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
}