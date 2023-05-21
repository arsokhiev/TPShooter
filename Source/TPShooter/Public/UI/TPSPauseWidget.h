// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "TPSBaseWidget.h"
#include "TPSPauseWidget.generated.h"

class UButton;

UCLASS()
class TPSHOOTER_API UTPSPauseWidget : public UTPSBaseWidget
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
