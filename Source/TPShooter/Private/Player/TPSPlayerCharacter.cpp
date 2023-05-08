// Third Player Shooter, All Rights Reserved


#include "Player/TPSPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TPSWeaponComponent.h"

ATPSPlayerCharacter::ATPSPlayerCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(this->GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ATPSPlayerCharacter::OnDeathHandle()
{
	Super::OnDeathHandle();
	
	if (Controller) // in Controller.h
	{
		//Controller takes control of the SpectatorPawn
		Controller->ChangeState(NAME_Spectating);
	}
}

void ATPSPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	ACharacter::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	check(WeaponComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATPSPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPSPlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ATPSPlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnAround", this, &ATPSPlayerCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ATPSPlayerCharacter::Jump);

	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &ATPSPlayerCharacter::OnStartRunning);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, this, &ATPSPlayerCharacter::OnStopRunning);

	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, WeaponComponent, &UTPSWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Released, WeaponComponent, &UTPSWeaponComponent::StopFire);

	PlayerInputComponent->BindAction("NextWeapon", EInputEvent::IE_Pressed, WeaponComponent,
	                                 &UTPSWeaponComponent::NextWeapon);
	PlayerInputComponent->BindAction("Reload", EInputEvent::IE_Pressed, WeaponComponent, &UTPSWeaponComponent::Reload);
}

bool ATPSPlayerCharacter::IsRunning() const
{
	return WantsToRun && IsMovingForward && !IsMovingSideward && !this->GetVelocity().IsZero();
}

void ATPSPlayerCharacter::MoveForward(float Scale)
{
	IsMovingForward = Scale > 0.0f;
	if (Scale == 0.0f) return;
	AddMovementInput(this->GetActorForwardVector(), Scale);
}

void ATPSPlayerCharacter::MoveRight(float Scale)
{
	IsMovingSideward = Scale > 0.0f || Scale < 0.0f;
	if (Scale == 0.0f) return;
	AddMovementInput(this->GetActorRightVector(), Scale);
}

void ATPSPlayerCharacter::OnStartRunning()
{
	WantsToRun = true;
}

void ATPSPlayerCharacter::OnStopRunning()
{
	WantsToRun = false;
}
