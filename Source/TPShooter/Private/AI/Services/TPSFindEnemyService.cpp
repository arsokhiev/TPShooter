// Third Player Shooter, All Rights Reserved


#include "AI/Services/TPSFindEnemyService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "TPSUtils.h"
#include "TPSAIPerceptionComponent.h"

UTPSFindEnemyService::UTPSFindEnemyService()
{
	NodeName = "Find Enemy";
}

//Every Tick in Blackboard fill the EnemyActorKey with the nearest enemy
void UTPSFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (const auto Blackboard = OwnerComp.GetBlackboardComponent())
	{
		const auto Controller = OwnerComp.GetAIOwner();
		if (const auto ControllerPerceptionComponent = TPSUtils::GetTPSPlayerComponent<UTPSAIPerceptionComponent>(Controller))
		{
			//write in EnemyActorKey (EnemyActor) closest enemy or nullptr
			Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, ControllerPerceptionComponent->GetClosestEnemy());
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}