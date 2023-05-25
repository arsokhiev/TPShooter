// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSGoToMenuWidget.generated.h"

class USoundCue;
class UButton;

UCLASS()
class TPSHOOTER_API UTPSGoToMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* GoToMenuButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MenuLoadingWidgetClass;

	virtual void NativeOnInitialized() override;
	
private:
	UFUNCTION()
	void OnGoToMenuHandle();
	
};
