// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Player/TPSBaseCharacter.h"
#include "TPSPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

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

	virtual void OnDeathHandle() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual bool IsRunning() const override;

private:
	bool WantsToRun = false;
	bool IsMovingForward;
	bool IsMovingSideward;

	void MoveForward(float Scale);
	void MoveRight(float Scale);

	void OnStartRunning();
	void OnStopRunning();
};
