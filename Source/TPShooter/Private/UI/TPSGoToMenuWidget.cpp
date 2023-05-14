// Third Player Shooter, All Rights Reserved


#include "UI/TPSGoToMenuWidget.h"
#include "TPSGameInstance.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogTPSGoToMenuWidget, All, All);

void UTPSGoToMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (GoToMenuButton)
	{
		GoToMenuButton->OnClicked.AddDynamic(this, &UTPSGoToMenuWidget::OnGoToMenuHandle);
	}
}

void UTPSGoToMenuWidget::OnGoToMenuHandle()
{
	if (!GetWorld()) return;

	const auto GameInstance = GetWorld()->GetGameInstance<UTPSGameInstance>();
	if (!GameInstance) return;

	if (GameInstance->GetMenuLevelName().IsNone())
	{
		UE_LOG(LogTPSGoToMenuWidget, Error, TEXT("Menu level name is NONE"));
		return;
	}

	UGameplayStatics::OpenLevel(this, GameInstance->GetMenuLevelName());
}
