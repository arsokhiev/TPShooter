// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "TPSCoreTypes.h"
#include "GameFramework/HUD.h"
#include "TPSGameHUD.generated.h"

class UTPSBaseWidget;

UCLASS()
class TPSHOOTER_API ATPSGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerHUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TMap<ETPSMatchState, UTPSBaseWidget*> GameWidgets;

	UPROPERTY()
	UTPSBaseWidget* CurrentWidget = nullptr;
	
	void OnMatchStateChangedHandle(ETPSMatchState State);
};
