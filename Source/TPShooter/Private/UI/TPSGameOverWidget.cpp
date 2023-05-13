// Third Player Shooter, All Rights Reserved


#include "UI/TPSGameOverWidget.h"
#include "TPSGameModeBase.h"
#include "TPSPlayerState.h"
#include "TPSPlayerStatRowWidget.h"
#include "Components/VerticalBox.h"
#include "TPSUtils.h"

bool UTPSGameOverWidget::Initialize()
{
	if (GetWorld())
	{
		const auto GameMode = Cast<ATPSGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->OnMatchStateChanged.AddUObject(this, &UTPSGameOverWidget::OnMatchStateChangedHandle);
		}
	}
	
	return Super::Initialize();
}

void UTPSGameOverWidget::OnMatchStateChangedHandle(ETPSMatchState State)
{
	if (State == ETPSMatchState::GameOver)
	{
		UpdatePlayerStat();
	}
}

void UTPSGameOverWidget::UpdatePlayerStat()
{
	if (!GetWorld() || !PlayerStatBox) return;

	PlayerStatBox->ClearChildren();

	for (auto ControllerIterator = GetWorld()->GetControllerIterator(); ControllerIterator; ++ControllerIterator)
	{
		const auto Controller = ControllerIterator->Get();
		if (!Controller) continue;

		const auto PlayerState = Cast<ATPSPlayerState>(Controller->PlayerState);
		if (!PlayerState) continue;

		const auto PlayerStatRowWidget = CreateWidget<UTPSPlayerStatRowWidget>(GetWorld(), PlayerStatRowWidgetClass);
		if (!PlayerStatRowWidget) continue;

		PlayerStatRowWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));
		PlayerStatRowWidget->SetKills(TPSUtils::TextFromInt(PlayerState->GetKillsNum()));
		PlayerStatRowWidget->SetDeaths(TPSUtils::TextFromInt(PlayerState->GetDeathsNum()));
		PlayerStatRowWidget->SetTeam(TPSUtils::TextFromInt(PlayerState->GetTeamID()));
		PlayerStatRowWidget->SetPlayerIndicatorVisibility(Controller->IsPlayerController());

		PlayerStatBox->AddChildToVerticalBox(PlayerStatRowWidget);
	}
}
