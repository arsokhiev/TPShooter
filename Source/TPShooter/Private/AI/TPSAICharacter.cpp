// Third Player Shooter, All Rights Reserved


#include "AI/TPSAICharacter.h"
#include "BrainComponent.h"
#include "AI/TPSAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/TPSAIWeaponComponent.h"

ATPSAICharacter::ATPSAICharacter(const FObjectInitializer& ObjInit)
	: Super(ObjInit.SetDefaultSubobjectClass<UTPSAIWeaponComponent>("WeaponComponent"))
{
	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = ATPSAIController::StaticClass();

	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
	}
}

void ATPSAICharacter::OnDeathHandle()
{
	Super::OnDeathHandle();

	const auto TPSAIController = Cast<ATPSAIController>(Controller);
	if (TPSAIController && TPSAIController->BrainComponent)
	{
		TPSAIController->BrainComponent->Cleanup();
	}
}
