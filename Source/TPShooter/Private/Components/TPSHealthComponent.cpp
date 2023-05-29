// Third Player Shooter, All Rights Reserved

#include "Components/TPSHealthComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "TPSGameModeBase.h"
#include "TPSUtils.h"
#include "Camera/CameraShakeBase.h"
#include "Perception/AISense_Damage.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)

void UTPSHealthComponent::HealUpdate()
{
	SetHealth(Health + HealModifier);

	if (IsHealthFull() && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
}

void UTPSHealthComponent::SetHealth(float NewHealth)
{
	const float NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	const float HealthDelta = NextHealth - Health;

	Health = NextHealth;
	OnHealthChanged.Broadcast(Health, HealthDelta);
}

void UTPSHealthComponent::PlayCameraShake()
{
	if (IsDead()) return;

	const APawn* Player = Cast<APawn>(GetOwner());
	if (!Player) return;

	const APlayerController* Controller = Player->GetController<APlayerController>();
	if (!Controller || !Controller->PlayerCameraManager) return;

	Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

void UTPSHealthComponent::Killed(AController* KillerController)
{
	if (!GetWorld()) return;

	const auto GameMode = Cast<ATPSGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!GameMode) return;

	const auto Pawn = Cast<APawn>(GetOwner());
	const auto VictimController = Pawn ? Pawn->GetController() : nullptr;

	GameMode->Killed(KillerController, VictimController);
}

void UTPSHealthComponent::ApplyDamage(float Damage, AController* InstigatedBy)
{
	// no teammate damage
	const auto Controller = Cast<APawn>(GetOwner())->GetController();
	if (!Controller || !InstigatedBy) return;
	if (TPSUtils::AreTeammates(Controller, InstigatedBy)) return;

	if (Damage <= 0.0f || IsDead() || !GetWorld()) return;

	SetHealth(Health - Damage);

	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

	if (IsDead())
	{
		Killed(InstigatedBy);
		OnDeath.Broadcast();
	}
	else if (AutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UTPSHealthComponent::HealUpdate, HealUpdateTime,
											   true, HealDelay);
	}

	PlayCameraShake();
	ReportDamageEvent(Damage, InstigatedBy);
}

float UTPSHealthComponent::GetPointDamageModifier(AActor* DamagedActor, const FName& BoneName)
{
	const auto Character = Cast<ACharacter>(DamagedActor);
	if (!Character || !Character->GetMesh() || !Character->GetMesh()->GetBodyInstance(BoneName)) return 1.0f;

	const auto PhysMaterial = Character->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial();
	if (!PhysMaterial || !DamageModifiers.Contains(PhysMaterial)) return 1.0f;

	return DamageModifiers[PhysMaterial];
}

void UTPSHealthComponent::ReportDamageEvent(float Damage, AController* InstigatedBy)
{
	if (!InstigatedBy || !InstigatedBy->GetPawn() || !GetOwner()) return;
	
	UAISense_Damage::ReportDamageEvent(GetWorld(),
										   GetOwner(),
										   InstigatedBy->GetPawn(),
										   Damage,
										   InstigatedBy->GetPawn()->GetActorLocation(),
										   GetOwner()->GetActorLocation());
}

UTPSHealthComponent::UTPSHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UTPSHealthComponent::TryToAddHealth(float HealthAmount)
{
	if (IsDead() || IsHealthFull()) return false;

	SetHealth(Health + HealthAmount);
	return true;
}

bool UTPSHealthComponent::IsHealthFull() const
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}

void UTPSHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxHealth > 0);
	SetHealth(MaxHealth);

	AActor* ComponentOwner = this->GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UTPSHealthComponent::OnTakeAnyDamageHandle);
		ComponentOwner->OnTakePointDamage.AddDynamic(this, &UTPSHealthComponent::OnTakePointDamageHandle);
		ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &UTPSHealthComponent::OnTakeRadialDamageHandle);
	}
}

void UTPSHealthComponent::OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                                AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogHealthComponent, Display, TEXT("On any damage: %f"), Damage);
}

void UTPSHealthComponent::OnTakePointDamageHandle(AActor* DamagedActor, float Damage, AController* InstigatedBy,
	FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
	const UDamageType* DamageType, AActor* DamageCauser)
{
	const auto FinalDamage = Damage * GetPointDamageModifier(DamagedActor, BoneName);
	UE_LOG(LogHealthComponent, Display, TEXT("On point damage: %f, bone: %s"), FinalDamage, *BoneName.ToString());
	ApplyDamage(FinalDamage, InstigatedBy);
}

void UTPSHealthComponent::OnTakeRadialDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	FVector Origin, FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogHealthComponent, Display, TEXT("On radial damage: %f"), Damage);
	ApplyDamage(Damage, InstigatedBy);
}
