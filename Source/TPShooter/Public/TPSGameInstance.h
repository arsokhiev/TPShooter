// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TPSGameInstance.generated.h"

UCLASS()
class TPSHOOTER_API UTPSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	FName GetStartupLevelName() const { return StartupLevelName; }
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FName StartupLevelName = NAME_None;
	
};
