// Third Player Shooter, All Rights Reserved


#include "Components/TPSAIPerceptionComponent.h"
#include "AIController.h"
#include "TPSUtils.h"
#include "Components/TPSHealthComponent.h"
#include "Perception/AISense_Sight.h"

AActor* UTPSAIPerceptionComponent::GetClosestEnemy() const
{
	//getting all perceived actor
	TArray<AActor*> PerceiveActors;
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceiveActors);
	if (PerceiveActors.Num() == 0) return nullptr;

	//getting a pawn controlled by ai controller
	const auto Controller = Cast<AAIController>(GetOwner());
	if (!Controller) return nullptr;
	const auto Pawn = Controller->GetPawn();
	if (!Pawn) return nullptr;

	//determining the nearest actor for a pawn controlled by ai controller
	float BestDistance = MAX_FLT;
	AActor* BestPawn = nullptr;

	for (const auto PerceiveActor : PerceiveActors)
	{
		const auto PerceiveActorHealthComponent = TPSUtils::GetTPSPlayerComponent<UTPSHealthComponent>(PerceiveActor);

		const auto PerceivePawn = Cast<APawn>(PerceiveActor);
		const auto AreEnemies = PerceivePawn && TPSUtils::AreEnemies(Controller, PerceivePawn->Controller);
		
		if (PerceiveActorHealthComponent && !PerceiveActorHealthComponent->IsDead() && AreEnemies)
		{
			const auto CurrentDistance = (PerceiveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
			if (CurrentDistance < BestDistance)
			{
				BestDistance = CurrentDistance;
				BestPawn = PerceiveActor;
			}
		}
	}

	return BestPawn;
}
