// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSGoToMenuWidget.generated.h"

class UButton;

UCLASS()
class TPSHOOTER_API UTPSGoToMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* GoToMenuButton;

	virtual void NativeOnInitialized() override;
	
private:
	UFUNCTION()
	void OnGoToMenuHandle();
	
};
