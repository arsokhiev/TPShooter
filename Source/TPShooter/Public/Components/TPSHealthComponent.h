// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPSHealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSHOOTER_API UTPSHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTPSHealthComponent();

	float GetHealth() const { return Health; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0", ClampMax = "150"));
	float MaxHealth = 100.0f;

private:
	float Health = 0.0f;
};
