// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "TPSCoreTypes.h"
#include "Blueprint/UserWidget.h"
#include "TPSGameOverWidget.generated.h"

class UVerticalBox;

UCLASS()
class TPSHOOTER_API UTPSGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

protected:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* PlayerStatBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> PlayerStatRowWidgetClass;

private:
	void OnMatchStateChangedHandle(ETPSMatchState State);
	void UpdatePlayerStat();
};
