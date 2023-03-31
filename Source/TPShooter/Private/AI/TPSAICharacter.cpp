// Third Player Shooter, All Rights Reserved


#include "AI/TPSAICharacter.h"
#include "TPSAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

ATPSAICharacter::ATPSAICharacter(const FObjectInitializer& ObjInit) : ATPSBaseCharacter(ObjInit)
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ATPSAIController::StaticClass();

	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
	}
}
