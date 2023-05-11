// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Player/TPSBaseCharacter.h"
#include "TPSPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USphereComponent;

UCLASS()
class TPSHOOTER_API ATPSPlayerCharacter : public ATPSBaseCharacter
{
	GENERATED_BODY()

public:
	ATPSPlayerCharacter(const FObjectInitializer& ObjInit);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USphereComponent* CameraCollisionComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Aiming")
	UCurveFloat* CurveFOV;

	virtual void BeginPlay() override;
	virtual void OnDeathHandle() override;

public:
	virtual void Tick(float DeltaSeconds) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual bool IsRunning() const override;

private:
	FOnTimelineFloat AimingTimelineProgress;
	
	bool WantsToRun = false;
	bool IsMovingForward;
	bool IsMovingSideward;

	FTimeline AimingTimeline;

	void MoveForward(float Scale);
	void MoveRight(float Scale);

	void OnStartRunning();
	void OnStopRunning();

	UFUNCTION()
	void OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                   const FHitResult& SweepResult);

	UFUNCTION()
	void OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void CheckCameraOverlap();

	void EnableZoom();
	void DisableZoom();
};
