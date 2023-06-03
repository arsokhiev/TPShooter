// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/TPSBaseWeapon.h"
#include "TPSRifleWeapon.generated.h"

class UTPSWeaponFXComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UAudioComponent;
class USoundCue;

UCLASS()
class TPSHOOTER_API ATPSRifleWeapon : public ATPSBaseWeapon
{
	GENERATED_BODY()

private:
	FTimerHandle ShootTimerHandle;

	UPROPERTY()
	UNiagaraComponent* MuzzleFXComponent;

	UPROPERTY()
	UAudioComponent* FireAudioComponent;

	void MakeDamage(const FHitResult& HitResult);
	virtual void PlayShootFeedback() override;

	void InitFX();
	void SetFXActive(bool IsActive);

	void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);

	AController* GetController() const;

	float CurrentBulletSpread;

public:
	ATPSRifleWeapon();
	
	virtual void StartFire() override;
	virtual void StopFire() override;
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

	void ChangeBulletSpread(float Spread) { CurrentBulletSpread = Spread; }
	float GetDefaultBulletSpread() const { return DefaultBulletSpread; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float DamageAmount = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	UForceFeedbackEffect* ShootFeedback;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TimeBetweenShoots = 0.15f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float DefaultBulletSpread = 0.7f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UTPSWeaponFXComponent* WeaponFXComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* TraceFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	FString TraceTargetName = "TraceTarget";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* FireEndSound;

	virtual void BeginPlay() override;
	virtual void MakeShoot() override;
	
};
