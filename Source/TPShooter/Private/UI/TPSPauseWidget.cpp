// Third Player Shooter, All Rights Reserved


#include "UI/TPSPauseWidget.h"
#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"

void UTPSPauseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ClearPauseButton)
	{
		ClearPauseButton->OnClicked.AddDynamic(this, &UTPSPauseWidget::OnClearPause);
	}
}

void UTPSPauseWidget::OnClearPause()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

	GetWorld()->GetAuthGameMode()->ClearPause();
}
