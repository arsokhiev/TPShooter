// Third Player Shooter, All Rights Reserved


#include "Player/TPSPlayerController.h"
#include "TPSGameModeBase.h"
#include "Components/TPSRespawnComponent.h"

ATPSPlayerController::ATPSPlayerController()
{
	RespawnComponent = CreateDefaultSubobject<UTPSRespawnComponent>("RespawnComponent");
}

void ATPSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		const auto GameMode = Cast<ATPSGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->OnMatchStateChanged.AddUObject(this, &ATPSPlayerController::OnMatchStateChangedHandle);
		}
	}
}

void ATPSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!InputComponent) return;
	InputComponent->BindAction("Pause", EInputEvent::IE_Pressed, this, &ATPSPlayerController::OnPauseGame);
}

void ATPSPlayerController::OnPauseGame()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;
	GetWorld()->GetAuthGameMode()->SetPause(this);
}

void ATPSPlayerController::OnMatchStateChangedHandle(ETPSMatchState State)
{
	if (State == ETPSMatchState::InProgress)
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
	}
}
