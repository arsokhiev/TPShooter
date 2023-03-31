// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Microsoft/AllowMicrosoftPlatformTypes.h"
#include "TPSProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UTPSWeaponFXComponent;

UCLASS()
class TPSHOOTER_API ATPSProjectile : public AActor
{
	GENERATED_BODY()

private:
	FVector NormalizedShootDirection;

	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
						 FVector NormalImpulse, const FHitResult& Hit);

	AController *GetController();

public:
	ATPSProjectile();

	void SetShootDirection(const FVector& NormalizedDirection) { NormalizedShootDirection = NormalizedDirection; }

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float DamageRadius = 400.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float DamageAmount = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	bool DoFullDamage = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float LifeSeconds = 30.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	UTPSWeaponFXComponent* WeaponFXComponent;

	virtual void BeginPlay() override;
};
