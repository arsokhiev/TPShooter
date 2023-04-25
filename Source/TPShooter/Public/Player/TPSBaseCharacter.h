// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSBaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UTPSHealthComponent;
class UTextRenderComponent;
class UTPSWeaponComponent;

UCLASS()
class TPSHOOTER_API ATPSBaseCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	bool WantsToRun = false;
	bool IsMovingForward;
	bool IsMovingSideward;

	void MoveForward(float Scale);
	void MoveRight(float Scale);

	void OnStartRunning();
	void OnStopRunning();

	void OnHealthChangedHandle(float Health, float HealthDelta) const;

	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit);

public:
	// Sets default values for this character's properties
	ATPSBaseCharacter(const FObjectInitializer& ObjInit);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UTPSHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UTextRenderComponent* HealthTextComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UTPSWeaponComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float LifeSpanOnDeath = 5.0f;

	// предельная скорость при которой будет нанесен ущерб
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

	// сам ущерб
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnDeathHandle();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsRunning() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetMovementDirection() const;
};
