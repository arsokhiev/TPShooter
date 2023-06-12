// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPSCoreTypes.h"
#include "TPSHealthComponent.generated.h"

class UCameraShakeBase;
class UPhysicalMaterial;

UENUM(BlueprintType)
enum class EHealthEffects : uint8
{
	IncreaseHealth = 0,
	DecreaseHealth
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TPSHOOTER_API UTPSHealthComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	float Health = 0.0f;
	FTimerHandle HealTimerHandle;

	UFUNCTION()
	void OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	                           class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void OnTakePointDamageHandle(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation,
	                       class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
	                       const class UDamageType* DamageType, AActor* DamageCauser);

	UFUNCTION()
	void OnTakeRadialDamageHandle(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin,
	                        const FHitResult& HitInfo, class AController* InstigatedBy, AActor* DamageCauser);

	void HealUpdate();
	void SetHealth(float NewHealth);
	void PlayCameraShake(EHealthEffects);

	void Killed(AController* KillerController);

	void ApplyDamage(float Damage, AController* InstigatedBy);
	float GetPointDamageModifier(AActor* DamagedActor, const FName& BoneName);

	void ReportDamageEvent(float Damage, AController* InstigatedBy);

public:
	UTPSHealthComponent();

	float GetHealth() const { return Health; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	bool IsDead() { return Health <= 0.0f; }

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthPercent() const { return Health / MaxHealth; }

	bool TryToAddHealth(float HealthAmount);
	bool IsHealthFull() const;

	FOnDeathSignature OnDeath;
	FOnHealthChangedSignature OnHealthChanged;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	TMap<UPhysicalMaterial*, float> DamageModifiers;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = 10.0f, ClampMax = 1000.0f));
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
	bool AutoHeal = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal",
		meta = (EditCondition = "AutoHeal", ClampMin = 0.001f, ClampMax = 3.0f));
	float HealUpdateTime = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal",
		meta = (EditCondition = "AutoHeal", ClampMin = 0.001f, ClampMax = 60.0f));
	float HealDelay = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal",
		meta = (EditCondition = "AutoHeal", ClampMin = 0.001f, ClampMax = 5.0f));
	float HealModifier = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	TSubclassOf<UCameraShakeBase> DamageCameraShake;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	TSubclassOf<UCameraShakeBase> HealCameraShake;
};
