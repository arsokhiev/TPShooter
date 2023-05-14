// Third Player Shooter, All Rights Reserved


#include "Menu/UI/TPSMenuHUD.h"
#include "Blueprint/UserWidget.h"
#include "Menu/UI/TPSMenuWidget.h"

void ATPSMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MenuWidgetClass)
	{
		const auto MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
		if (MenuWidget)
		{
			MenuWidget->AddToViewport();
		}
	}
}
