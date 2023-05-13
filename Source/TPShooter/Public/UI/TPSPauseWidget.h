// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSPauseWidget.generated.h"

class UButton;

UCLASS()
class TPSHOOTER_API UTPSPauseWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* ClearPauseButton;

	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnClearPause();
};
