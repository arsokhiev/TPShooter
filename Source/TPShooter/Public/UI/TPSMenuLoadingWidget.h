// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/TPSBaseWidget.h"
#include "TPSMenuLoadingWidget.generated.h"

class USoundCue;

UCLASS()
class TPSHOOTER_API UTPSMenuLoadingWidget : public UTPSBaseWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
};
