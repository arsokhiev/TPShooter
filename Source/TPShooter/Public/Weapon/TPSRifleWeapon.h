// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/TPSBaseWeapon.h"
#include "TPSRifleWeapon.generated.h"

class UTPSWeaponFXComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class TPSHOOTER_API ATPSRifleWeapon : public ATPSBaseWeapon
{
	GENERATED_BODY()

private:
	FTimerHandle ShootTimerHandle;

	UPROPERTY()
	UNiagaraComponent* MuzzleFXComponent;

	void MakeDamage(const FHitResult& HitResult);
	virtual void PlayShootFeedback() override;

	void InitMuzzleFX();
	void SetMuzzleFXVisibility(bool Visible);

	void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);

public:
	ATPSRifleWeapon();
	
	virtual void StartFire() override;
	virtual void StopFire() override;
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float DamageAmount = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	UForceFeedbackEffect* ShootFeedback;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TimeBetweenShoots = 0.15f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float BulletSpread = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UTPSWeaponFXComponent* WeaponFXComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* TraceFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	FString TraceTargetName = "TraceTarget";

	virtual void BeginPlay() override;
	virtual void MakeShoot() override;
	
};
