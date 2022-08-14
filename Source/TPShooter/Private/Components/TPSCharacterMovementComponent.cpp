// Third Player Shooter, All Rights Reserved


#include "Components/TPSCharacterMovementComponent.h"
#include "Player/TPSBaseCharacter.h"

float UTPSCharacterMovementComponent::GetMaxSpeed() const
{
	//поучил MaxSpeed из базового класса
	const float MaxSpeed = UCharacterMovementComponent::GetMaxSpeed();

	//возвращает указатель на паун(“ѕ—„арактер)
	const ATPSBaseCharacter* Player = Cast<ATPSBaseCharacter>(this->GetPawnOwner());
	return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}