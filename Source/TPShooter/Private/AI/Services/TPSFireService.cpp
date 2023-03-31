// Third Player Shooter, All Rights Reserved


#include "AI/Services/TPSFireService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/TPSWeaponComponent.h"
#include "TPSUtils.h"

UTPSFireService::UTPSFireService()
{
	NodeName = "Fire";
}

void UTPSFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto AIController = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();

	const auto HasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

	if (AIController)
	{
		const auto AIWeaponComponent = TPSUtils::GetTPSPlayerComponent<UTPSWeaponComponent>(AIController->GetPawn());
		if (AIWeaponComponent)
		{
			HasAim ? AIWeaponComponent->StartFire() : AIWeaponComponent->StopFire();
		}
	}
	
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
