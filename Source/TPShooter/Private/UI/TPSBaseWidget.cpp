// Third Player Shooter, All Rights Reserved


#include "UI/TPSBaseWidget.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

void UTPSBaseWidget::Show()
{
	PlayAnimation(ShowAnimation);
	UGameplayStatics::PlaySound2D(GetWorld(), OpenSound);
}
