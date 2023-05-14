// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TPSMenuHUD.generated.h"

UCLASS()
class TPSHOOTER_API ATPSMenuHUD : public AHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MenuWidgetClass;

	virtual void BeginPlay() override;
	
};
