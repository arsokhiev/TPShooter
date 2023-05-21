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
	
	virtual void NativeOnInitialized() override;

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
