// Third Player Shooter, All Rights Reserved


#include "UI/TPSMenuLoadingWidget.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "TPSGameInstance.h"

void UTPSMenuLoadingWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (OpenSound && ShowAnimation)
	{
		PlayAnimation(ShowAnimation);
		UGameplayStatics::PlaySound2D(GetWorld(), OpenSound);
	}
}

void UTPSMenuLoadingWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);

	if (Animation != ShowAnimation) return;
	
	if (!GetWorld()) return;
	const auto TPSGameInstance = GetWorld()->GetGameInstance<UTPSGameInstance>();
	
	if (!TPSGameInstance) return;
	UGameplayStatics::OpenLevel(this, TPSGameInstance->GetMenuLevelName());
}
