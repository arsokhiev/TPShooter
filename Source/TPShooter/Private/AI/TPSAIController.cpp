// Third Player Shooter, All Rights Reserved


#include "AI/TPSAIController.h"
#include "AI/TPSAICharacter.h"
#include "Components/TPSAIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ATPSAIController::ATPSAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UTPSAIPerceptionComponent>("TPSPerceptionComponent");
	SetPerceptionComponent(*AIPerceptionComponent);

	bWantsPlayerState = true;
}

void ATPSAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (const auto TPSAICharacter = Cast<ATPSAICharacter>(InPawn))
	{
		RunBehaviorTree(TPSAICharacter->BehaviorTreeAsset);
	}
}

void ATPSAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const auto AimActor = GetFocusOnActor();
	SetFocus(AimActor);
}

//Will turn to FocusOnKeyName (EnemyActor)
//if an enemy pointer was written to EnemyActorKey (EnemyActor) in the TPSFindEnemyService service
AActor* ATPSAIController::GetFocusOnActor() const
{
	if (!GetBlackboardComponent()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}