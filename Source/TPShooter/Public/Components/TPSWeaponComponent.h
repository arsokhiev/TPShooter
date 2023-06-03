// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPSCoreTypes.h"
#include "TPSWeaponComponent.generated.h"

class ATPSBaseWeapon;
class UCameraShakeBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TPSHOOTER_API UTPSWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UAnimMontage* CurrentReloadAnimMontage = nullptr;
	
	bool EquipAnimInProgress = false;
	bool ReloadAnimInProgress = false;

	void SpawnWeapons();
	void AttachWeaponToSocket(ATPSBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
	
	void PlayAnimMontage(UAnimMontage* AnimMontage) const;
	void InitAnimations();
	void OnEquipFinished(USkeletalMeshComponent* MeshComp);
	void OnReloadFinished(USkeletalMeshComponent* MeshComp);
	
	bool CanReload() const;

	void OnEmptyClip(ATPSBaseWeapon* AmmoEmptyWeapon);
	void ChangeClip();

	void PlayCameraShake() const;

public:
	UTPSWeaponComponent();

	virtual void StartFire();
	void StopFire();
	bool IsFiring() const;

	virtual void NextWeapon();

	void Reload();

	ATPSBaseWeapon* GetCurrentWeapon() const;
	bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;
	bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

	bool TryToAddAmmo(TSubclassOf<ATPSBaseWeapon> WeaponType, int32 ClipsAmount);
	bool NeedAmmo(TSubclassOf<ATPSBaseWeapon> WeaponType);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<FWeaponData> WeaponData;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponEquipSocketName = "WeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponArmorySocketName = "ArmorySocket";

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* EquipAnimMontage;

	UPROPERTY()
	ATPSBaseWeapon* CurrentWeapon = nullptr;

	UPROPERTY()
	TArray<ATPSBaseWeapon*> Weapons;
	
	int32 CurrentWeaponIndex = 0;
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	bool CanFire();
	bool CanEquip() const;
	void EquipWeapon(int32 WeaponIndex);
};