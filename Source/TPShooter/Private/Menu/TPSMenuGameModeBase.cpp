// Third Player Shooter, All Rights Reserved


#include "Menu/TPSMenuGameModeBase.h"
#include "Menu/TPSMenuPlayerController.h"
#include "Menu/UI/TPSMenuHUD.h"

ATPSMenuGameModeBase::ATPSMenuGameModeBase()
{
	PlayerControllerClass = ATPSMenuPlayerController::StaticClass();
	HUDClass = ATPSMenuHUD::StaticClass();
}
