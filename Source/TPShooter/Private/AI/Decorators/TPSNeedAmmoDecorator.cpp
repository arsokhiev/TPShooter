// Third Player Shooter, All Rights Reserved


#include "AI/Decorators/TPSNeedAmmoDecorator.h"
#include "AIController.h"
#include "TPSAICharacter.h"
#include "TPSAIWeaponComponent.h"
#include "TPSBaseWeapon.h"
#include "TPSUtils.h"

UTPSNeedAmmoDecorator::UTPSNeedAmmoDecorator()
{
	NodeName = "Need Ammo";
}

bool UTPSNeedAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto AIController = OwnerComp.GetAIOwner();
	if (!AIController) return false;

	const auto WeaponComponent = TPSUtils::GetTPSPlayerComponent<UTPSAIWeaponComponent>(AIController->GetPawn());
	if (!WeaponComponent) return false;

	return WeaponComponent->NeedAmmo(WeaponType);
}