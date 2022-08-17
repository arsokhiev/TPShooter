// Third Player Shooter, All Rights Reserved


#include "Components/TPSHealthComponent.h"
#include "GameFramework/Actor.h"

UTPSHealthComponent::UTPSHealthComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts
void UTPSHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	//params: 1. указатель на текущий instance | 2. функция-подписчик
	this->GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UTPSHealthComponent::OnTakeAnyDamageHandle);
}


void UTPSHealthComponent::OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	Health -= Damage;
}