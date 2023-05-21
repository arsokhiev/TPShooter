// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "TPSCoreTypes.h"
#include "TPSBaseWidget.h"
#include "TPSGameOverWidget.generated.h"

class UVerticalBox;
class UButton;

UCLASS()
class TPSHOOTER_API UTPSGameOverWidget : public UTPSBaseWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* PlayerStatBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerStatRowWidgetClass;

	UPROPERTY(meta = (BindWidget))
	UButton* ResetLevelButton;

	virtual void NativeOnInitialized() override;

private:
	void OnMatchStateChangedHandle(ETPSMatchState State);
	void UpdatePlayerStat();

	UFUNCTION()
	void OnResetLevelHandle();
};
