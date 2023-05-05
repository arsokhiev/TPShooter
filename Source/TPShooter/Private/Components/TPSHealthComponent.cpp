// Third Player Shooter, All Rights Reserved

#include "Components/TPSHealthComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Dev/TPSFireDamageType.h"
#include "Dev/TPSIceDamageType.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "TPSGameModeBase.h"
#include "TPSUtils.h"
#include "Camera/CameraShakeBase.h"

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
	}
}

void UTPSHealthComponent::OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                                AController* InstigatedBy, AActor* DamageCauser)
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
}
