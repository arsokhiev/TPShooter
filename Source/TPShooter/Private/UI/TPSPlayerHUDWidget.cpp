// Third Player Shooter, All Rights Reserved


#include "UI/TPSPlayerHUDWidget.h"
#include "Components/TPSHealthComponent.h"
#include "Components/TPSWeaponComponent.h"
#include "TPSUtils.h"

float UTPSPlayerHUDWidget::GetHealthPercent() const
{
	const UTPSHealthComponent* HealthComponent = TPSUtils::GetTPSPlayerComponent<UTPSHealthComponent>(GetOwningPlayerPawn());
	if (!HealthComponent) return 0.0f;
	
	return HealthComponent->GetHealthPercent();
}

bool UTPSPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
	const UTPSWeaponComponent* WeaponComponent = TPSUtils::GetTPSPlayerComponent<UTPSWeaponComponent>(GetOwningPlayerPawn());
	if (!WeaponComponent) return false;
	
	return WeaponComponent->GetCurrentWeaponUIData(UIData);
}

bool UTPSPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData)
{
	const UTPSWeaponComponent* WeaponComponent = TPSUtils::GetTPSPlayerComponent<UTPSWeaponComponent>(GetOwningPlayerPawn());
	if (!WeaponComponent) return false;
	
	return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
}

bool UTPSPlayerHUDWidget::IsPlayerAlive() const
{
	UTPSHealthComponent* HealthComponent = TPSUtils::GetTPSPlayerComponent<UTPSHealthComponent>(GetOwningPlayerPawn());
	return HealthComponent && !HealthComponent->IsDead();
}

bool UTPSPlayerHUDWidget::IsPlayerSpectating() const
{
	const auto Controller = GetOwningPlayer();
	return Controller && Controller->GetStateName() == NAME_Spectating;
}

bool UTPSPlayerHUDWidget::Initialize()
{
	if (GetOwningPlayer())
	{
		GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &UTPSPlayerHUDWidget::OnNewPawnHandle);
		//Explicitly called, because on the first spawn, the OnNewPawn delegate is not broadcast
		OnNewPawnHandle(GetOwningPlayerPawn());
	}
	return Super::Initialize();
}

void UTPSPlayerHUDWidget::OnHealthChanged(float Health, float HealthDelta)
{
	if (HealthDelta < 0.0f)
	{
		OnTakeDamage();
	}
}

void UTPSPlayerHUDWidget::OnNewPawnHandle(APawn* NewPawn)
{
	if (UTPSHealthComponent* const HealthComponent = TPSUtils::GetTPSPlayerComponent<UTPSHealthComponent>(NewPawn))
	{
		HealthComponent->OnHealthChanged.AddUObject(this, &UTPSPlayerHUDWidget::OnHealthChanged);
	}
}
