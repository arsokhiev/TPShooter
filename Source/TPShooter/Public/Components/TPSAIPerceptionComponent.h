// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "TPSAIPerceptionComponent.generated.h"

UCLASS()
class TPSHOOTER_API UTPSAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
	AActor* GetClosestEnemy() const;
	
};
