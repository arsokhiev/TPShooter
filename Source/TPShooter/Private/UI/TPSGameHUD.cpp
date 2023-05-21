// Third Player Shooter, All Rights Reserved


#include "UI/TPSGameHUD.h"
#include "TPSGameModeBase.h"
#include "Engine/Canvas.h"
#include "UI/TPSBaseWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogTPSGameHUD, All, All);

void ATPSGameHUD::DrawHUD()
{
	Super::DrawHUD();
}

void ATPSGameHUD::BeginPlay()
{
	Super::BeginPlay();

	GameWidgets.Add(ETPSMatchState::InProgress, CreateWidget<UTPSBaseWidget>(GetWorld(), PlayerHUDWidgetClass));
	GameWidgets.Add(ETPSMatchState::Pause, CreateWidget<UTPSBaseWidget>(GetWorld(), PauseWidgetClass));
	GameWidgets.Add(ETPSMatchState::GameOver, CreateWidget<UTPSBaseWidget>(GetWorld(), GameOverWidgetClass));

	for (auto GameWidgetPair : GameWidgets)
	{
		const auto Widget = GameWidgetPair.Value;
		if (!Widget) continue;
		Widget->AddToViewport();
		Widget->SetVisibility(ESlateVisibility::Hidden);
	}
	
	if (GetWorld())
	{
		const auto GameMode = Cast<ATPSGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->OnMatchStateChanged.AddUObject(this, &ATPSGameHUD::OnMatchStateChangedHandle);
		}
	}
}

void ATPSGameHUD::OnMatchStateChangedHandle(ETPSMatchState State)
{
	if (CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (GameWidgets.Contains(State))
	{
		CurrentWidget = GameWidgets[State];
	}

	if (CurrentWidget)
	{
		CurrentWidget->SetVisibility(ESlateVisibility::Visible);
		CurrentWidget->Show();
	}
	
	UE_LOG(LogTPSGameHUD, Display, TEXT("Match state changed: %s"), *UEnum::GetValueAsString(State));
}
