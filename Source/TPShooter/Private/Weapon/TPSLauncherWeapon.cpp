// Third Player Shooter, All Rights Reserved


#include "Weapon/TPSLauncherWeapon.h"
#include "GameFramework/Character.h"
#include "Weapon/TPSProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void ATPSLauncherWeapon::PlayShootFeedback()
{
	APlayerController* Controller = GetPlayerController();
	Controller->ClientPlayForceFeedback(ShootFeedback);
}

void ATPSLauncherWeapon::StartFire()
{
	MakeShoot();
}

void ATPSLauncherWeapon::MakeShoot()
{
	if (!GetWorld()) return;

	if (IsAmmoEmpty())
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), NoProjectilesSound, GetActorLocation());
		return;
	}

	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) return;

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	const FVector StartPoint = GetMuzzleWorldLocation();

	//todo попадание если стоит позади
	const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;//TraceEnd;
	const FVector NormalizedDirection = (EndPoint - StartPoint).GetSafeNormal();

	const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
	ATPSProjectile* Projectile = GetWorld()->SpawnActorDeferred<ATPSProjectile>(ProjectileClass, SpawnTransform);
	if (Projectile)
	{
		Projectile->SetOwner(GetOwner());
		Projectile->SetShootDirection(NormalizedDirection);
		Projectile->FinishSpawning(SpawnTransform);
	}

	DecreaseAmmo();
	SpawnMuzzleFX();

	UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);

	Super::MakeShoot();
}
