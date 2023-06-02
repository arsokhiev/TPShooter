// Third Player Shooter, All Rights Reserved


#include "Components/TPSWeaponComponent.h"
#include "TPSBaseCharacter.h"
#include "TPSHealthComponent.h"
#include "TPSPlayerCharacter.h"
#include "TPSUtils.h"
#include "Weapon/TPSBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animations/TPSEquipFinishedAnimNotify.h"
#include "Animations/TPSReloadFinishedAnimNotify.h"
#include "Animations/AnimUtils.h"
#include "Camera/CameraShakeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All)

constexpr static int32 WeaponNum = 2;

UTPSWeaponComponent::UTPSWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTPSWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(WeaponData.Num() == 2, TEXT("Character can hold only %i weapon items"), WeaponNum);
	
	InitAnimations();
	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);
}

void UTPSWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CurrentWeapon = nullptr;
	for (auto Weapon : Weapons)
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->Destroy();
	}
	Weapons.Empty();

	Super::EndPlay(EndPlayReason);
}

void UTPSWeaponComponent::SpawnWeapons()
{
	ATPSBaseCharacter* Character = Cast<ATPSBaseCharacter>(GetOwner());
	if (!Character || !GetWorld()) return;

	for (auto OneWeaponData : WeaponData)
	{
		ATPSBaseWeapon* Weapon = GetWorld()->SpawnActor<ATPSBaseWeapon>(OneWeaponData.WeaponClass);
		if (!Weapon) continue;

		Weapon->OnClipEmpty.AddUObject(this, &UTPSWeaponComponent::OnEmptyClip);
		Weapon->OnShootMade.AddUObject(this, &UTPSWeaponComponent::PlayCameraShake);
		Weapon->SetOwner(Character);
		Weapons.Add(Weapon);

		AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
	}
}

void UTPSWeaponComponent::AttachWeaponToSocket(ATPSBaseWeapon* Weapon, USceneComponent* SceneComponent,
                                               const FName& SocketName)
{
	if (!Weapon || !SceneComponent) return;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void UTPSWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
	if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
	{
		UE_LOG(LogWeaponComponent, Display, TEXT("Invalid weapon index"));
		return;
	}

	ACharacter* Character = Cast<ATPSBaseCharacter>(GetOwner());
	if (!Character || !GetWorld()) return;

	//skipped on first call as current weapon will be nullptr
	if (CurrentWeapon)
	{
		StopFire();
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
	}

	CurrentWeapon = Weapons[WeaponIndex];
	//CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;
	const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data)
	{
		return Data.WeaponClass == CurrentWeapon->GetClass();
	});
	CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr; 

	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);

	EquipAnimInProgress = true;
	PlayAnimMontage(EquipAnimMontage);
}

void UTPSWeaponComponent::PlayAnimMontage(UAnimMontage* AnimMontage) const
{
	ACharacter* Character = Cast<ATPSBaseCharacter>(GetOwner());
	if (!Character || !GetWorld()) return;

	Character->PlayAnimMontage(AnimMontage);
}

void UTPSWeaponComponent::InitAnimations()
{
	if (const auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<UTPSEquipFinishedAnimNotify>(EquipAnimMontage))
	{
		EquipFinishedNotify->OnNotified.AddUObject(this, &UTPSWeaponComponent::OnEquipFinished);
	}
	else
	{
		UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is not set"));
		checkNoEntry();
	}

	for (auto OneWeaponData : WeaponData)
	{
		auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<UTPSReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
		if (!ReloadFinishedNotify)
		{
			UE_LOG(LogWeaponComponent, Error, TEXT("Reload anim notify is not set"));
			checkNoEntry();
		}
		ReloadFinishedNotify->OnNotified.AddUObject(this, &UTPSWeaponComponent::OnReloadFinished);
	}
}

void UTPSWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
	ACharacter* Character = Cast<ATPSBaseCharacter>(GetOwner());
	if (!Character || Character->GetMesh() != MeshComp) return;

	EquipAnimInProgress = false;
	UE_LOG(LogWeaponComponent, Display, TEXT("Equip finished"));
}

void UTPSWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComp)
{
	ACharacter* Character = Cast<ATPSBaseCharacter>(GetOwner());
	if (!Character || Character->GetMesh() != MeshComp) return;

	ReloadAnimInProgress = false;
	UE_LOG(LogWeaponComponent, Display, TEXT("Reload finished"));
}

bool UTPSWeaponComponent::CanFire()
{
	ATPSBaseCharacter* Character = Cast<ATPSBaseCharacter>(GetOwner());
	if (Character && Character->IsRunning())
	{
		if (CurrentWeapon) StopFire();
		return false;
	}
	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool UTPSWeaponComponent::CanEquip() const
{
	return !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool UTPSWeaponComponent::CanReload() const
{
	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress && CurrentWeapon->CanReload();
}

void UTPSWeaponComponent::OnEmptyClip(ATPSBaseWeapon* AmmoEmptyWeapon)
{
	if (!AmmoEmptyWeapon) return;
	
	if (CurrentWeapon == AmmoEmptyWeapon)
	{
		ChangeClip();
	}
	else
	{
		for (const auto Weapon : Weapons)
		{
			if (Weapon == AmmoEmptyWeapon)
			{
				Weapon->ChangeClip();
			}
		}
	}
}

void UTPSWeaponComponent::ChangeClip()
{
	if (!CanReload()) return;

	CurrentWeapon->StopFire();
	CurrentWeapon->ChangeClip();
	
	ReloadAnimInProgress = true;
	PlayAnimMontage(CurrentReloadAnimMontage);
}

void UTPSWeaponComponent::PlayCameraShake() const
{
	const auto Character = Cast<ATPSBaseCharacter>(GetOwner());
	if (!Character) return;
	if (TPSUtils::GetTPSPlayerComponent<UTPSHealthComponent>(Character)->IsDead()) return;

	const auto Controller = Character->GetController<APlayerController>();
	if (!Controller || !Controller->PlayerCameraManager) return;

	if (!CurrentWeapon->CameraShake) return;
	Controller->PlayerCameraManager->StartCameraShake(CurrentWeapon->CameraShake);
}

void UTPSWeaponComponent::StartFire()
{
	if (!CanFire()) return;
	CurrentWeapon->StartFire();
}

void UTPSWeaponComponent::StopFire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StopFire();
}

bool UTPSWeaponComponent::IsFiring() const
{
	return CurrentWeapon && CurrentWeapon->IsFiring();
}

void UTPSWeaponComponent::NextWeapon()
{
	if (!CanEquip()) return;
	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	EquipWeapon(CurrentWeaponIndex);
}

void UTPSWeaponComponent::Reload()
{
	ChangeClip();
}

bool UTPSWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
	if (CurrentWeapon)
	{
		UIData = CurrentWeapon->GetUIData();
		return true;
	}
	return false;
}

bool UTPSWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
	if (CurrentWeapon)
	{
		AmmoData = CurrentWeapon->GetAmmoData();
		return true;
	}
	return false;
}

bool UTPSWeaponComponent::TryToAddAmmo(TSubclassOf<ATPSBaseWeapon> WeaponType, int32 ClipsAmount)
{
	for (const auto Weapon : Weapons)
	{
		if (Weapon && Weapon->IsA(WeaponType))
		{
			return Weapon->TryToAddAmmo(ClipsAmount);
		}
	}
	return false;
}

bool UTPSWeaponComponent::NeedAmmo(TSubclassOf<ATPSBaseWeapon> WeaponType)
{
	for (const auto Weapon : Weapons)
	{
		if (Weapon && Weapon->IsA(WeaponType))
		{
			return !Weapon->IsAmmoFull();
		}
	}
	return false;
}
