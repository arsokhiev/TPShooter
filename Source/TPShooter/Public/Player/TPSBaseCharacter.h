// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSBaseCharacter.generated.h"

class UTPSHealthComponent;
class UTPSWeaponComponent;

UCLASS()
class TPSHOOTER_API ATPSBaseCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	void OnHealthChangedHandle(float Health, float HealthDelta) const;

	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit);

public:
	// Sets default values for this character's properties
	ATPSBaseCharacter(const FObjectInitializer& ObjInit);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UTPSHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UTPSWeaponComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float LifeSpanOnDeath = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

	UPROPERTY(EditAnywhere, Category = "Material")
	FName MaterialColorName = "Paint Color";

	virtual void BeginPlay() override;

	virtual void OnDeathHandle();

public:
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual bool IsRunning() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetMovementDirection() const;

	void SetPlayerColor(const FLinearColor& Color);
};
