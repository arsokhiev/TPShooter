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

public:
	virtual bool Initialize() override;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* ClearPauseButton;

private:
	UFUNCTION()
	void OnClearPause();
};
