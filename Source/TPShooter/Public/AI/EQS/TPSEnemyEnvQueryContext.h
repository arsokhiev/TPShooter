// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "TPSEnemyEnvQueryContext.generated.h"

UCLASS()
class TPSHOOTER_API UTPSEnemyEnvQueryContext : public UEnvQueryContext
{
	GENERATED_BODY()

public :
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FName EnemyActorKeyName = "EnemyActor";
};
