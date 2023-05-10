// Third Player Shooter, All Rights Reserved


#include "Player/TPSPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TPSWeaponComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

ATPSPlayerCharacter::ATPSPlayerCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(this->GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
	CameraCollisionComponent->SetupAttachment(CameraComponent);
	CameraCollisionComponent->SetSphereRadius(10.0f);
	CameraCollisionComponent->SetCollisionResponseToChannels(ECollisionResponse::ECR_Overlap);
}

void ATPSPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(CameraCollisionComponent);

	CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATPSPlayerCharacter::OnCameraCollisionBeginOverlap);
	CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ATPSPlayerCharacter::OnCameraCollisionEndOverlap);
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

	PlayerInputComponent->BindAction("NextWeapon", EInputEvent::IE_Pressed, WeaponComponent,&UTPSWeaponComponent::NextWeapon);
	PlayerInputComponent->BindAction("Reload", EInputEvent::IE_Pressed, WeaponComponent, &UTPSWeaponComponent::Reload);

	PlayerInputComponent->BindAction("Zoom", EInputEvent::IE_Pressed, WeaponComponent, &UTPSWeaponComponent::EnableZoom);
	PlayerInputComponent->BindAction("Zoom", EInputEvent::IE_Released, WeaponComponent, &UTPSWeaponComponent::DisableZoom);
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

void ATPSPlayerCharacter::OnCameraCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                        bool bFromSweep, const FHitResult& SweepResult)
{
	CheckCameraOverlap();
}

void ATPSPlayerCharacter::OnCameraCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckCameraOverlap();
}

void ATPSPlayerCharacter::CheckCameraOverlap()
{
	const auto bComponentsOverlap = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
	GetMesh()->SetOwnerNoSee(bComponentsOverlap);

	TArray<USceneComponent*> MeshChildren;
	GetMesh()->GetChildrenComponents(true, MeshChildren);

	for (auto MeshChild : MeshChildren)
	{
		const auto PrimitiveMeshChild = Cast<UPrimitiveComponent>(MeshChild);
		if (PrimitiveMeshChild)
		{
			PrimitiveMeshChild->SetOwnerNoSee(bComponentsOverlap);
		}
	}
}
