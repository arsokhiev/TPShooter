// Third Player Shooter, All Rights Reserved


#include "Pickup/TPSHealthPickup.h"
#include "Components/TPSHealthComponent.h"
#include "TPSUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);

bool ATPSHealthPickup::GivePickupTo(APawn* PlayerPawn)
{
	const auto HealthComponent = TPSUtils::GetTPSPlayerComponent<UTPSHealthComponent>(PlayerPawn);
	if (!HealthComponent || HealthComponent->IsDead()) return false;
	
	return HealthComponent->TryToAddHealth(HealthAddAmount);
}