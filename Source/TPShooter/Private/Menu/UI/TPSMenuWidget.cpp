// Third Player Shooter, All Rights Reserved


#include "Menu/UI/TPSMenuWidget.h"
#include "TPSGameInstance.h"
#include "TPSLevelItemWidget.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogTPSMenuWidget, All, All);

void UTPSMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &UTPSMenuWidget::OnStartGameHandle);
	}

	if (QuitGameButton)
	{
		QuitGameButton->OnClicked.AddDynamic(this, &UTPSMenuWidget::OnQuitGameHandle);
	}

	InitLevelItems();
}

void UTPSMenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);

	if (Animation != HideAnimation) return;
	
	const auto TPSGameInstance = GetTPSGameInstance();
	if (!TPSGameInstance) return;
	
	UGameplayStatics::OpenLevel(this, TPSGameInstance->GetStartupLevel().LevelName);
}

void UTPSMenuWidget::OnStartGameHandle()
{
	PlayAnimation(HideAnimation);
}

void UTPSMenuWidget::OnQuitGameHandle()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

void UTPSMenuWidget::InitLevelItems()
{
	const auto TPSGameInstance = GetTPSGameInstance();
	if (!TPSGameInstance) return;

	checkf(TPSGameInstance->GetLevelsData().Num() != 0, TEXT("Levels data must not be empty"));

	if (!LevelItemsBox) return;
	LevelItemsBox->ClearChildren();

	for (auto LevelData : TPSGameInstance->GetLevelsData())
	{
		const auto LevelItemWidget = CreateWidget<UTPSLevelItemWidget>(GetWorld(), LevelItemWidgetClass);
		if (!LevelItemWidget) continue;

		LevelItemWidget->SetLevelData(LevelData);
		LevelItemWidget->OnLevelSelected.AddUObject(this, &UTPSMenuWidget::OnLevelSelectedHandle);

		LevelItemsBox->AddChild(LevelItemWidget);
		LevelItemWidgets.Add(LevelItemWidget);
	}

	if (TPSGameInstance->GetStartupLevel().LevelName.IsNone())
	{
		OnLevelSelectedHandle(TPSGameInstance->GetLevelsData()[0]);
	}
	else
	{
		OnLevelSelectedHandle(TPSGameInstance->GetStartupLevel());
	}
}

void UTPSMenuWidget::OnLevelSelectedHandle(const FLevelData& Data)
{
	const auto TPSGameInstance = GetTPSGameInstance();
	if (!TPSGameInstance) return;

	TPSGameInstance->SetStartupLevel(Data);

	for (auto LevelItemWidget : LevelItemWidgets)
	{
		if (LevelItemWidget)
		{
			const auto IsSelected = Data.LevelName == LevelItemWidget->GetLevelData().LevelName;
			LevelItemWidget->SetSelected(IsSelected);
		}
	}
}

UTPSGameInstance* UTPSMenuWidget::GetTPSGameInstance() const
{
	if (!GetWorld()) return nullptr;
	return GetWorld()->GetGameInstance<UTPSGameInstance>();
}
