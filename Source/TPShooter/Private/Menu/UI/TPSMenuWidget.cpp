// Third Player Shooter, All Rights Reserved


#include "Menu/UI/TPSMenuWidget.h"

#include "TPSGameInstance.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogTPSMenuWidget, All, All);

void UTPSMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &UTPSMenuWidget::OnStartGameHandle);
	}
}

void UTPSMenuWidget::OnStartGameHandle()
{
	if (!GetWorld()) return;

	const auto GameInstance = GetWorld()->GetGameInstance<UTPSGameInstance>();
	if (!GameInstance) return;

	if (GameInstance->GetStartupLevelName().IsNone())
	{
		UE_LOG(LogTPSMenuWidget, Error, TEXT("Level name is NONE"));
		return;
	}
	
	UGameplayStatics::OpenLevel(this, GameInstance->GetStartupLevelName());
}
