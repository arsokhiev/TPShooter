// Third Player Shooter, All Rights Reserved


#include "Pickup/TPSBasePickup.h"
#include "Components/SphereComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBasePickup, All, All);

// Sets default values
ATPSBasePickup::ATPSBasePickup()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(50.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetRootComponent(CollisionComponent);
}

void ATPSBasePickup::BeginPlay()
{
	Super::BeginPlay();

	check(CollisionComponent);
	GenerateRotationYaw();
}

void ATPSBasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (GivePickupTo(Cast<APawn>(OtherActor)))
	{
		PickupWasTaken();
	}
}

void ATPSBasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0.0f, RotationYaw * DeltaTime * 100, 0.0f));
}

void ATPSBasePickup::GenerateRotationYaw()
{
	const auto Direction = FMath::RandBool() ? 1.0f : -1.0f;
	RotationYaw = FMath::RandRange(1.0f, 2.0f) * Direction;
}

bool ATPSBasePickup::GivePickupTo(APawn* PlayerPawn)
{
	return false;
}

void ATPSBasePickup::PickupWasTaken()
{
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	if (GetRootComponent())
	{
		GetRootComponent()->SetVisibility(false, true);
	}

	FTimerHandle RespawnTimerHandle;
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ATPSBasePickup::Respawn, RespawnTime);
}

void ATPSBasePickup::Respawn()
{
	GenerateRotationYaw();
	if (GetRootComponent())
	{
		GetRootComponent()->SetVisibility(true, true);
	}
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}
