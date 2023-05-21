// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSBaseWidget.generated.h"

UCLASS()
class TPSHOOTER_API UTPSBaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Show();
	
protected:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ShowAnimation;
};
