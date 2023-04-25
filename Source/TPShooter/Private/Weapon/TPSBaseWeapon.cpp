// Third Player Shooter, All Rights Reserved


#include "Weapon/TPSBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

// Sets default values
ATPSBaseWeapon::ATPSBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}

void ATPSBaseWeapon::StartFire() {}

void ATPSBaseWeapon::StopFire() {}

void ATPSBaseWeapon::PlayShootFeedback() {}

void ATPSBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponMesh);

	checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count should`nt be less or equal zero"));
	checkf(DefaultAmmo.Clips > 0, TEXT("Clips count should`nt be less or equal zero"));
	
	CurrentAmmo = DefaultAmmo;
}

void ATPSBaseWeapon::MakeShoot()
{
	if (Cast<ACharacter>(GetOwner())->IsPlayerControlled())
	{
		PlayShootFeedback();
		OnShootMade.Broadcast();
	}
}

APlayerController* ATPSBaseWeapon::GetPlayerController() const
{
	ACharacter* Player = Cast<ACharacter>(GetOwner());
	if (!Player) return nullptr;

	return Player->GetController<APlayerController>();
}

bool ATPSBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	const auto TPSCharacter = Cast<ACharacter>(GetOwner());
	if (!TPSCharacter) return false;
	
	if (TPSCharacter->IsPlayerControlled())
	{
		const auto PlayerController = GetPlayerController();
		if (!PlayerController) return false;

		PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}
	else
	{
		ViewLocation = GetMuzzleWorldLocation();
		ViewRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
	}
	
	return true;
}

FVector ATPSBaseWeapon::GetMuzzleWorldLocation() const
{
	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

FVector ATPSBaseWeapon::GetMuzzleForwardVector() const
{
	return WeaponMesh->GetSocketTransform(MuzzleSocketName).GetRotation().GetForwardVector();
}

//трейс из камеры в центр экрана (прямо)
bool ATPSBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	//const FTransform MuzzleSocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);

	TraceStart = ViewLocation;
	const FVector NormalizedShootDirection = ViewRotation.Vector();
	TraceEnd = TraceStart + (NormalizedShootDirection * TraceMaxDistance);
	return true;
}

//записывает в HitResult, было ли пересечение по каналу Visibility на отрезке от TraceStart до TraceEnd
void ATPSBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
	if (!GetWorld()) return;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	CollisionParams.bReturnPhysicalMaterial = true; // запись в HitResult
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility,
	                                     CollisionParams);
}

void ATPSBaseWeapon::DecreaseAmmo()
{
	if (CurrentAmmo.Bullets == 0)
	{
		UE_LOG(LogBaseWeapon, Warning, TEXT("Clip is empty"));
		return;
	}
	
	--CurrentAmmo.Bullets;
	LogAmmo();

	if (IsClipEmpty() && !IsAmmoEmpty())
	{
		StopFire();
		OnClipEmpty.Broadcast(this);
		//ChangeClip();
	}
}

bool ATPSBaseWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool ATPSBaseWeapon::IsClipEmpty() const
{
	return CurrentAmmo.Bullets == 0;
}

bool ATPSBaseWeapon::IsAmmoFull() const
{
	return CurrentAmmo.Clips == DefaultAmmo.Clips && CurrentAmmo.Bullets == DefaultAmmo.Bullets;
}

void ATPSBaseWeapon::ChangeClip()
{
	if (!CurrentAmmo.Infinite)
	{
		if (CurrentAmmo.Clips == 0)
		{
			UE_LOG(LogBaseWeapon, Warning, TEXT("No more clips"));
			return;
		}
		--CurrentAmmo.Clips;
	}

	CurrentAmmo.Bullets = DefaultAmmo.Bullets;
	UE_LOG(LogBaseWeapon, Display, TEXT("-----------------Change Clip-------------------"));
}

bool ATPSBaseWeapon::CanReload() const
{
	return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}

bool ATPSBaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
	if (CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0) return false;

	if (IsAmmoEmpty())
	{
		UE_LOG(LogBaseWeapon, Display, TEXT("Ammo was empty"));
		// +1 потому что далее идет перезарядка и одна обойма идет в оружие
		CurrentAmmo.Clips = FMath::Clamp(ClipsAmount, 0, DefaultAmmo.Clips + 1); 
		OnClipEmpty.Broadcast(this);
	}
	else if (CurrentAmmo.Clips < DefaultAmmo.Clips)
	{
		const auto NextClipsAmount = CurrentAmmo.Clips = ClipsAmount;
		if (DefaultAmmo.Clips - ClipsAmount >= 0)
		{
			UE_LOG(LogBaseWeapon, Display, TEXT("Clips were added"));
			CurrentAmmo.Clips = NextClipsAmount;
		}
		else
		{
			UE_LOG(LogBaseWeapon, Display, TEXT("Ammo is full now"));
			CurrentAmmo.Clips = DefaultAmmo.Clips;
			CurrentAmmo.Bullets = DefaultAmmo.Bullets;
		}
	}
	else
	{
		UE_LOG(LogBaseWeapon, Display, TEXT("Bullets were added"));
		CurrentAmmo.Bullets = DefaultAmmo.Bullets;
	}

	return true;
}

void ATPSBaseWeapon::LogAmmo()
{
	FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + "/";
	AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
	UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}

UNiagaraComponent* ATPSBaseWeapon::SpawnMuzzleFX()
{
	return UNiagaraFunctionLibrary::SpawnSystemAttached(
		MuzzleFX,
		WeaponMesh,
		MuzzleSocketName,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget, true);
}