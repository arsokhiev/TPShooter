// Third Player Shooter, All Rights Reserved


#include "UI/TPSPlayerHUDWidget.h"
#include "Components/TPSHealthComponent.h"
#include "Components/TPSWeaponComponent.h"
#include "TPSUtils.h"
#include "Components/ProgressBar.h"

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

int32 UTPSPlayerHUDWidget::GetKillsNum() const
{
	const auto PlayerState = GetTPSPlayerState();
	return PlayerState ? PlayerState->GetKillsNum() : 0;
}

FString UTPSPlayerHUDWidget::FormatBullets(int32 BulletsNum) const
{
	const int32 MaxLen = 2;
	const TCHAR PrefixSym = '0';

	auto BulletsStr = FString::FromInt(BulletsNum);

	if (const auto SymbolsNumToAdd = MaxLen - BulletsStr.Len(); SymbolsNumToAdd > 0)
	{
		BulletsStr = FString::ChrN(SymbolsNumToAdd, PrefixSym).Append(BulletsStr);
	}

	return BulletsStr;
}

void UTPSPlayerHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (GetOwningPlayer())
	{
		GetOwningPlayer()->GetOnNewPawnNotifier().AddUObject(this, &UTPSPlayerHUDWidget::OnNewPawnHandle);
		//Explicitly called, because on the first spawn, the OnNewPawn delegate is not broadcast
		OnNewPawnHandle(GetOwningPlayerPawn());
	}
}

void UTPSPlayerHUDWidget::OnHealthChanged(float Health, float HealthDelta)
{
	if (HealthDelta < 0.0f)
	{
		//OnTakeDamage(); rewritten in c++ below

		if (!IsAnimationPlaying(DamageAnimation))
		{
			PlayAnimation(DamageAnimation);
		}
	}

	UpdateHealthBar();
}

void UTPSPlayerHUDWidget::OnNewPawnHandle(APawn* NewPawn)
{
	if (UTPSHealthComponent* const HealthComponent = TPSUtils::GetTPSPlayerComponent<UTPSHealthComponent>(NewPawn))
	{
		HealthComponent->OnHealthChanged.AddUObject(this, &UTPSPlayerHUDWidget::OnHealthChanged);
	}

	UpdateHealthBar();
}

void UTPSPlayerHUDWidget::UpdateHealthBar()
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetFillColorAndOpacity(GetHealthPercent() > PercentColorThreshold ? GoodColor : BadColor);
	}
}

ATPSPlayerState* UTPSPlayerHUDWidget::GetTPSPlayerState() const
{
	return GetOwningPlayer() ? Cast<ATPSPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
}
