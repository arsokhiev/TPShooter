// Third Player Shooter, All Rights Reserved


#include "UI/TPSPauseWidget.h"
#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"

bool UTPSPauseWidget::Initialize()
{
	const auto InitResult = Super::Initialize();

	if (ClearPauseButton)
	{
		ClearPauseButton->OnClicked.AddDynamic(this, &UTPSPauseWidget::OnClearPause);
	}

	return InitResult;
}

void UTPSPauseWidget::OnClearPause()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

	GetWorld()->GetAuthGameMode()->ClearPause();
}
