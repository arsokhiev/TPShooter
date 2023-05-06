// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TPSAIController.generated.h"

class UTPSAIPerceptionComponent;
class UTPSRespawnComponent;

UCLASS()
class TPSHOOTER_API ATPSAIController : public AAIController
{
	GENERATED_BODY()

public:
	ATPSAIController();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UTPSAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UTPSRespawnComponent* RespawnComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName FocusOnKeyName = "EnemyActor";
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;

private:
	AActor* GetFocusOnActor() const;	
};