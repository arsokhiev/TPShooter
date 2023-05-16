// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Player/TPSBaseCharacter.h"
#include "TPSAICharacter.generated.h"

class UBehaviorTree;
class UWidgetComponent;

UCLASS()
class TPSHOOTER_API ATPSAICharacter : public ATPSBaseCharacter
{
	GENERATED_BODY()

public:
	ATPSAICharacter(const FObjectInitializer& ObjInit);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	float HealthVisibilityDistance = 1000.0f;

	virtual void BeginPlay() override;
	
	virtual void OnDeathHandle() override;
	virtual void OnHealthChangedHandle(float Health, float HealthDelta) const override;

private:
	void UpdateHealthWidgetVisibility();
};
