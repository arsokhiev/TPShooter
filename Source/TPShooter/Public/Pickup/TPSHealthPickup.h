// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Pickup/TPSBasePickup.h"
#include "TPSHealthPickup.generated.h"

UCLASS()
class TPSHOOTER_API ATPSHealthPickup : public ATPSBasePickup
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "0.0", ClampMax = "500.0"))
	int32 HealthAddAmount = 50;

private:
	virtual bool GivePickupTo(APawn* PlayerPawn) override;
};