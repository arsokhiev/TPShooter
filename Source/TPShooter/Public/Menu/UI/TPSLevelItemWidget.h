// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSCoreTypes.h"
#include "TPSLevelItemWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;

UCLASS()
class TPSHOOTER_API UTPSLevelItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnLevelSelectedSignature OnLevelSelected;

	void SetLevelData(const FLevelData& Data);
	const FLevelData& GetLevelData() const { return LevelData; }

	void SetSelected(bool IsSelected);
	
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* LevelSelectButton;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* LevelNameTextBlock;
	
	UPROPERTY(meta = (BindWidget))
	UImage* LevelImage = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	UImage* FrameImage;

	virtual void NativeOnInitialized() override;

private:
	FLevelData LevelData;

	UFUNCTION()
	void OnLevelItemClickedHandle();
};
