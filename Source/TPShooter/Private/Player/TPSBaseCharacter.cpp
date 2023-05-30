// Third Player Shooter, All Rights Reserved


#include "Player/TPSBaseCharacter.h"
#include "Components/TPSHealthComponent.h"
#include "Components/TPSCharacterMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "PhysicsProxy/GeometryCollectionPhysicsProxy.h"
#include "GameFramework/Controller.h"
#include "Components/TPSWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All)

ATPSBaseCharacter::ATPSBaseCharacter(const FObjectInitializer& ObjInit)
	: ACharacter(ObjInit.SetDefaultSubobjectClass<UTPSCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UTPSHealthComponent>("HealthComponent");
	WeaponComponent = CreateDefaultSubobject<UTPSWeaponComponent>("WeaponComponent");
}	

void ATPSBaseCharacter::BeginPlay()
{
	ACharacter::BeginPlay();

	check(HealthComponent);
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

bool ATPSBaseCharacter::IsRunning() const
{
	return false;
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

void ATPSBaseCharacter::SetPlayerColor(const FLinearColor& Color)
{
	const auto MaterialInstance = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);
	if (!MaterialInstance) return;

	MaterialInstance->SetVectorParameterValue(MaterialColorName, Color);
}

void ATPSBaseCharacter::OnDeathHandle()
{
	this->GetCharacterMovement()->DisableMovement();
	SetLifeSpan(LifeSpanOnDeath);

	//Sets the collision response to be the same for all channels
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	WeaponComponent->StopFire();

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
}

void ATPSBaseCharacter::OnHealthChangedHandle(float Health, float HealthDelta) const
{
	
}

void ATPSBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
	//falling velocity
	const auto FallVelocityZ = -GetVelocity().Z;
	UE_LOG(LogBaseCharacter, Display, TEXT("On landed: %.0f"), FallVelocityZ);

	if (FallVelocityZ < LandedDamageVelocity.X) return;

	const auto FinalFallDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
	UE_LOG(LogBaseCharacter, Display, TEXT("Final Fall Damage: %.0f"), FinalFallDamage);
	TakeDamage(FinalFallDamage, FPointDamageEvent {}, nullptr, nullptr);
}