// Third Player Shooter, All Rights Reserved


#include "Weapon/TPSRifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Weapon/Components/TPSWeaponFXComponent.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraShakeBase.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(LogRifleWeapon, All, All);

void ATPSRifleWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamageTaker = HitResult.GetActor();
	if (!DamageTaker) return;

	DamageTaker->TakeDamage(DamageAmount, FDamageEvent{}, GetPlayerController(), this);
}

void ATPSRifleWeapon::PlayShootFeedback()
{
	APlayerController* Controller = GetPlayerController();
	Controller->ClientPlayForceFeedback(ShootFeedback);
}

void ATPSRifleWeapon::InitMuzzleFX()
{
	if (!MuzzleFXComponent)
	{
		MuzzleFXComponent = SpawnMuzzleFX();
	}
	SetMuzzleFXVisibility(true);
}

void ATPSRifleWeapon::SetMuzzleFXVisibility(bool Visible)
{
	if (MuzzleFXComponent)
	{
		MuzzleFXComponent->SetPaused(!Visible);
		MuzzleFXComponent->SetVisibility(Visible, true);
	}
}

void ATPSRifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
	const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
	if (TraceFXComponent)
	{
		TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
	}
}

ATPSRifleWeapon::ATPSRifleWeapon()
{
	WeaponFXComponent = CreateDefaultSubobject<UTPSWeaponFXComponent>("WeaponFXComponent");
}

void ATPSRifleWeapon::StartFire()
{
	UE_LOG(LogRifleWeapon, Display, TEXT("FIRE!!!!"));

	InitMuzzleFX();
	GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &ATPSRifleWeapon::MakeShoot, TimeBetweenShoots, true);
	MakeShoot();
}

void ATPSRifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShootTimerHandle);
	SetMuzzleFXVisibility(false);
}

//trace from the camera to center of the screen (straight) with some spread
bool ATPSRifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation;
	const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
	const FVector NormalizedShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
	TraceEnd = TraceStart + (NormalizedShootDirection * TraceMaxDistance);
	return true;
}

void ATPSRifleWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponFXComponent);
}

void ATPSRifleWeapon::MakeShoot()
{
	if (!GetWorld() || IsAmmoEmpty())
	{
		StopFire();
		return;
	}
	
	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd))
	{
		StopFire();
		return;
	}

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	FVector TraceFXEnd = TraceEnd;

	if (HitResult.bBlockingHit)
	{
		if (FMath::RadiansToDegrees(FMath::Acos(
					FVector::DotProduct(GetMuzzleForwardVector(), HitResult.ImpactPoint - GetMuzzleWorldLocation()))) > 90)
						return;
		
		TraceFXEnd = HitResult.ImpactPoint;
		MakeDamage(HitResult);
		WeaponFXComponent->PlayImpactFX(HitResult);

		/*UE_LOG(LogRifleWeapon, Display, TEXT("Bone: %s"), *HitResult.BoneName.ToString());
		 *DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
		 *DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Purple, false, 5.0f);*/
	}

	SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);

	DecreaseAmmo();
	
	Super::MakeShoot();
}