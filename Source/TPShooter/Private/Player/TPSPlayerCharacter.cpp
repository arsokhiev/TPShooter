// Third Player Shooter, All Rights Reserved


#include "Player/TPSPlayerCharacter.h"
#include "TPSRifleWeapon.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TPSWeaponComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"

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
	check(CurveFOV);

	CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATPSPlayerCharacter::OnCameraCollisionBeginOverlap);
	CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ATPSPlayerCharacter::OnCameraCollisionEndOverlap);

	AimingTimelineProgress.BindUFunction(CameraComponent, FName("SetFieldOfView"));
}

void ATPSPlayerCharacter::OnDeathHandle()
{
	Super::OnDeathHandle();

	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
}

void ATPSPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	AimingTimeline.TickTimeline(DeltaSeconds);
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

	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &ATPSPlayerCharacter::OnStartFire);
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Released, WeaponComponent, &UTPSWeaponComponent::StopFire);

	PlayerInputComponent->BindAction("NextWeapon", EInputEvent::IE_Pressed, WeaponComponent,&UTPSWeaponComponent::NextWeapon);
	PlayerInputComponent->BindAction("Reload", EInputEvent::IE_Pressed, WeaponComponent, &UTPSWeaponComponent::Reload);

	PlayerInputComponent->BindAction("Zoom", EInputEvent::IE_Pressed, this, &ATPSPlayerCharacter::EnableZoom);
	PlayerInputComponent->BindAction("Zoom", EInputEvent::IE_Released, this, &ATPSPlayerCharacter::DisableZoom);
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

	if (IsRunning() && WeaponComponent->IsFiring())
	{
		WeaponComponent->StopFire();
	}
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

	if (IsRunning())
	{
		WeaponComponent->StopFire();
	}
}

void ATPSPlayerCharacter::OnStopRunning()
{
	WantsToRun = false;
}

void ATPSPlayerCharacter::OnStartFire()
{
	if (IsRunning()) return;
	WeaponComponent->StartFire();
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

void ATPSPlayerCharacter::EnableZoom()
{
	AimingTimeline.AddInterpFloat(CurveFOV, AimingTimelineProgress);
	AimingTimeline.PlayFromStart();
	
	if (WeaponComponent && WeaponComponent->GetCurrentWeapon() &&
		WeaponComponent->GetCurrentWeapon()->IsA(ATPSRifleWeapon::StaticClass()))
	{
		const auto CurrentWeapon = Cast<ATPSRifleWeapon>(WeaponComponent->GetCurrentWeapon());
		CurrentWeapon->ChangeBulletSpread(0.0);
	}
}

void ATPSPlayerCharacter::DisableZoom()
{
	AimingTimeline.AddInterpFloat(CurveFOV, AimingTimelineProgress);
	AimingTimeline.ReverseFromEnd();

	if (WeaponComponent && WeaponComponent->GetCurrentWeapon() &&
		WeaponComponent->GetCurrentWeapon()->IsA(ATPSRifleWeapon::StaticClass()))
	{
		const auto CurrentWeapon = Cast<ATPSRifleWeapon>(WeaponComponent->GetCurrentWeapon());
		CurrentWeapon->ChangeBulletSpread(CurrentWeapon->GetDefaultBulletSpread());
	}
}
