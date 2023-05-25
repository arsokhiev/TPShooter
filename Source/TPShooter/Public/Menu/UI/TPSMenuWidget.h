// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "TPSBaseWidget.h"
#include "TPSCoreTypes.h"
#include "TPSMenuWidget.generated.h"

class UButton;
class UHorizontalBox;
class UTPSGameInstance;
class UTPSLevelItemWidget;
class USoundCue;

UCLASS()
class TPSHOOTER_API UTPSMenuWidget : public UTPSBaseWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* StartGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitGameButton;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* LevelItemsBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> LevelItemWidgetClass;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* HideAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* StartGameSound;
	
	virtual void NativeOnInitialized() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
	
private:
	UPROPERTY()
	TArray<UTPSLevelItemWidget*> LevelItemWidgets;;
	
	UFUNCTION()
	void OnStartGameHandle();

	UFUNCTION()
	void OnQuitGameHandle();

	void InitLevelItems();
	void OnLevelSelectedHandle(const FLevelData& Data);
	UTPSGameInstance* GetTPSGameInstance() const;
};
