// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Pickup/TPSBasePickup.h"
#include "TPSAmmoPickup.generated.h"

class ATPSBaseWeapon;

UCLASS()
class TPSHOOTER_API ATPSAmmoPickup : public ATPSBasePickup
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "10.0"))
	int32 ClipsAmount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	TSubclassOf<ATPSBaseWeapon> WeaponType;

private:
	virtual bool GivePickupTo(APawn* PlayerPawn) override;
};