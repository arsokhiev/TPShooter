// Third Player Shooter, All Rights Reserved


#include "AI/Decorators/TPSHealthPercentDecorator.h"
#include "AIController.h"
#include "TPSHealthComponent.h"
#include "TPSUtils.h"

UTPSHealthPercentDecorator::UTPSHealthPercentDecorator()
{
	NodeName = "Health Percent";
}

bool UTPSHealthPercentDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto AIController = OwnerComp.GetAIOwner();
	if (!AIController) return false;
	
	const auto HealthComponent = TPSUtils::GetTPSPlayerComponent<UTPSHealthComponent>(AIController->GetPawn());
	if (!HealthComponent || HealthComponent->IsDead()) return false;

	return HealthComponent->GetHealthPercent() <= HealthPercent;
}
