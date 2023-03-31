// Third Player Shooter, All Rights Reserved


#include "Player/TPSBaseCharacter.h"
#include "Components/TPSHealthComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TPSCharacterMovementComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/DamageEvents.h"
#include "PhysicsProxy/GeometryCollectionPhysicsProxy.h"
#include "GameFramework/Controller.h"
#include "Components/TPSWeaponComponent.h"
#include "Components/CapsuleComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All)

// Sets default values
ATPSBaseCharacter::ATPSBaseCharacter(const FObjectInitializer& ObjInit)
	: ACharacter(ObjInit.SetDefaultSubobjectClass<UTPSCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(this->GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthComponent = CreateDefaultSubobject<UTPSHealthComponent>("HealthComponent");
	
	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextComponent->SetupAttachment(GetRootComponent());
	HealthTextComponent->SetOwnerNoSee(true);

	WeaponComponent = CreateDefaultSubobject<UTPSWeaponComponent>("WeaponComponent");
}	

void ATPSBaseCharacter::BeginPlay()
{
	ACharacter::BeginPlay();

	check(HealthComponent);
	check(HealthTextComponent);
	check(GetCharacterMovement());
	check(GetMesh());

	OnHealthChangedHandle(HealthComponent->GetHealth(), 0.0f); 
	HealthComponent->OnDeath.AddUObject(this, &ATPSBaseCharacter::OnDeathHandle);
	HealthComponent->OnHealthChanged.AddUObject(this, &ATPSBaseCharacter::OnHealthChangedHandle);

	LandedDelegate.AddDynamic(this, &ATPSBaseCharacter::OnGroundLanded);
}

void ATPSBaseCharacter::Tick(float DeltaTime)
{
	ACharacter::Tick(DeltaTime);
}

void ATPSBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	ACharacter::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);
	check(WeaponComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATPSBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPSBaseCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ATPSBaseCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnAround", this, &ATPSBaseCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ATPSBaseCharacter::Jump);
	
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &ATPSBaseCharacter::OnStartRunning);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, this, &ATPSBaseCharacter::OnStopRunning);

	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, WeaponComponent, &UTPSWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Released, WeaponComponent, &UTPSWeaponComponent::StopFire);

	PlayerInputComponent->BindAction("NextWeapon", EInputEvent::IE_Pressed, WeaponComponent, &UTPSWeaponComponent::NextWeapon);
	PlayerInputComponent->BindAction("Reload", EInputEvent::IE_Pressed, WeaponComponent, &UTPSWeaponComponent::Reload);
}

bool ATPSBaseCharacter::IsRunning() const
{
	return WantsToRun && IsMovingForward && !IsMovingSideward && !this->GetVelocity().IsZero();
}

float ATPSBaseCharacter::GetMovementDirection() const
{
	if (this->GetVelocity().IsZero()) return 0.0f;
	
	const auto NormalizedVelocity = this->GetVelocity().GetSafeNormal();
	const auto ForwardVector = this->GetActorForwardVector();

	const auto AngleDegrees = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ForwardVector, NormalizedVelocity)));
	const auto CrossProduct = FVector::CrossProduct(ForwardVector, NormalizedVelocity);

	const auto ZSign = FMath::Sign(CrossProduct.Z);
	
	return CrossProduct.IsZero() ? AngleDegrees : AngleDegrees * ZSign;
}

void ATPSBaseCharacter::MoveForward(float Scale)
{
	IsMovingForward = Scale > 0.0f;
	if (Scale == 0.0f) return;
	AddMovementInput(this->GetActorForwardVector(), Scale);
}

void ATPSBaseCharacter::MoveRight(float Scale)
{
	IsMovingSideward = Scale > 0.0f || Scale < 0.0f;
	if (Scale == 0.0f) return;
	AddMovementInput(this->GetActorRightVector(), Scale);
}

void ATPSBaseCharacter::OnStartRunning()
{
	WantsToRun = true;
}

void ATPSBaseCharacter::OnStopRunning()
{
	WantsToRun = false;
}

void ATPSBaseCharacter::OnDeathHandle()
{
	//PlayAnimMontage(DeathAnimMontage);

	this->GetCharacterMovement()->DisableMovement();
	SetLifeSpan(LifeSpanOnDeath);

	if (Controller) // in Controller.h
	{
		//Controller takes control of the SpectatorPawn
		Controller->ChangeState(NAME_Spectating);
	}

	//Sets the collision response to be the same for all channels
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	WeaponComponent->StopFire();

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
}

void ATPSBaseCharacter::OnHealthChangedHandle(float Health, float HealthDelta) const
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void ATPSBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
	//falling velocity
	const auto FallVelocityZ = -GetVelocity().Z;
	UE_LOG(LogBaseCharacter, Display, TEXT("On landed: %.0f"), FallVelocityZ);

	if (FallVelocityZ < LandedDamageVelocity.X) return;

	const auto FinalFallDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
	UE_LOG(LogBaseCharacter, Display, TEXT("Final Fall Damage: %.0f"), FinalFallDamage);
	TakeDamage(FinalFallDamage, FDamageEvent {}, nullptr, nullptr);
}