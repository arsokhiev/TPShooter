// Third Player Shooter, All Rights Reserved


#include "TPSGameModeBase.h"
#include "TPSBaseCharacter.h"
#include "TPSPlayerController.h"
#include "TPSGameHUD.h"

ATPSGameModeBase::ATPSGameModeBase()
{
	DefaultPawnClass = ATPSBaseCharacter::StaticClass();
	PlayerControllerClass = ATPSPlayerController::StaticClass();
	HUDClass = ATPSGameHUD::StaticClass();
}
