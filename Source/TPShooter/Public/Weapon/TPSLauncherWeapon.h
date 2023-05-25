// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/TPSBaseWeapon.h"
#include "TPSLauncherWeapon.generated.h"

class ATPSProjectile;
class USoundCue;

UCLASS()
class TPSHOOTER_API ATPSLauncherWeapon : public ATPSBaseWeapon
{
	GENERATED_BODY()

private:
	virtual void PlayShootFeedback() override;

public:
	virtual void StartFire() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ATPSProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	UForceFeedbackEffect* ShootFeedback;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* NoProjectilesSound;
	
	virtual void MakeShoot() override;
};
