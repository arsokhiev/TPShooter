// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSAmbientSoundActor.generated.h"

UCLASS()
class TPSHOOTER_API ATPSAmbientSoundActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATPSAmbientSoundActor();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UAudioComponent* AudioComponent;
	
	virtual void BeginPlay() override;
};