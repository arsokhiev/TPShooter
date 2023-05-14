// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TPSMenuPlayerController.generated.h"

UCLASS()
class TPSHOOTER_API ATPSMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
};
