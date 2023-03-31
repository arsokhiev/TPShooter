// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSBasePickup.generated.h"

class USphereComponent;

UCLASS()
class TPSHOOTER_API ATPSBasePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	ATPSBasePickup();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
	USphereComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
	float RespawnTime = 5.0f;
	
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	float RotationYaw = 0.0f;
	void GenerateRotationYaw();
	
	virtual bool GivePickupTo(APawn* PlayerPawn);
	
	void PickupWasTaken();
	void Respawn();
};