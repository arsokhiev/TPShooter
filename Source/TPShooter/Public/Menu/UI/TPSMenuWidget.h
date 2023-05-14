// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSMenuWidget.generated.h"

class UButton;

UCLASS()
class TPSHOOTER_API UTPSMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* StartGameButton;
	
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnStartGameHandle();
	
};
