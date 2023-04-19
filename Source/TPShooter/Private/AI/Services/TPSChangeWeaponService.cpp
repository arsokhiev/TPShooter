// Third Player Shooter, All Rights Reserved


#include "AI/Services/TPSChangeWeaponService.h"
#include "AIController.h"
#include "TPSUtils.h"
#include "TPSWeaponComponent.h"

UTPSChangeWeaponService::UTPSChangeWeaponService()
{
	NodeName = "Change Weapon";
}

void UTPSChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (const auto AIController = OwnerComp.GetAIOwner())
	{
		const auto AIWeaponComponent = TPSUtils::GetTPSPlayerComponent<UTPSWeaponComponent>(AIController->GetPawn());
		if (AIWeaponComponent && Probability > 0 && FMath::FRand() <= Probability)
		{
			AIWeaponComponent->NextWeapon();
		}
	}
	
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
