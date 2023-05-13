// Third Player Shooter, All Rights Reserved


#include "TPSGameModeBase.h"
#include "EngineUtils.h"
#include "TPSBaseCharacter.h"
#include "TPSPlayerController.h"
#include "TPSGameHUD.h"
#include "TPSAIController.h"
#include "TPSPlayerCharacter.h"
#include "TPSPlayerState.h"
#include "TPSRespawnComponent.h"
#include "TPSUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogTPSGameModeBase, All, All);

ATPSGameModeBase::ATPSGameModeBase()
{
	DefaultPawnClass = ATPSPlayerCharacter::StaticClass();
	PlayerControllerClass = ATPSPlayerController::StaticClass();
	HUDClass = ATPSGameHUD::StaticClass();
	PlayerStateClass = ATPSPlayerState::StaticClass();
}

void ATPSGameModeBase::StartPlay()
{
	Super::StartPlay();

	SpawnBots();
	CreateTeamsInfo();

	CurrentRound = 1;
	StartRound();

	SetMatchState(ETPSMatchState::InProgress);
}

UClass* ATPSGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (InController && InController->IsA<AAIController>())
	{
		return AIPawnClass;
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ATPSGameModeBase::Killed(AController* KillerController, AController* VictimController)
{
	const auto KillerPlayerState = KillerController ? Cast<ATPSPlayerState>(KillerController->PlayerState) : nullptr;
	const auto VictimPlayerState = VictimController ? Cast<ATPSPlayerState>(VictimController->PlayerState) : nullptr;

	if (KillerPlayerState)
	{
		KillerPlayerState->AddKill();
	}

	if (VictimPlayerState)
	{
		VictimPlayerState->AddDeath();
	}

	StartRespawn(VictimController);
}

void ATPSGameModeBase::RespawnRequest(AController* Controller)
{
	ResetOnePlayer(Controller);
}

bool ATPSGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);
	if (PauseSet)
	{
		SetMatchState(ETPSMatchState::Pause);
	}
	
	return PauseSet;
}

bool ATPSGameModeBase::ClearPause()
{
	const auto PauseCleared = Super::ClearPause();

	if (PauseCleared)
	{
		SetMatchState(ETPSMatchState::InProgress);
	}
	
	return PauseCleared;
}

void ATPSGameModeBase::SpawnBots()
{
	if (!GetWorld()) return;

	for (int32 i = 0; i < GameData.PlayersNum - 1; ++i)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		const auto TPSAIController = GetWorld()->SpawnActor<ATPSAIController>(AIControllerClass, SpawnParameters);
		RestartPlayer(TPSAIController);
	}
}

void ATPSGameModeBase::StartRound()
{
	RoundCountDown = GameData.RoundTime;
	GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ATPSGameModeBase::GameTimerUpdate, 1.0f, true);
}

void ATPSGameModeBase::GameTimerUpdate()
{
	UE_LOG(LogTPSGameModeBase, Display, TEXT("Time: %i / Round: %i/%i"), RoundCountDown, CurrentRound, GameData.RoundsNum);
	
	if (--RoundCountDown == 0)
	{
		GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);

		if (CurrentRound + 1 <= GameData.RoundsNum)
		{
			++CurrentRound;
			ResetPlayers();
			StartRound();
		}
		else
		{
			GameOver();
		}
	}
}

void ATPSGameModeBase::ResetPlayers()
{
	if (!GetWorld()) return;

	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		ResetOnePlayer(It->Get());
	}
}

void ATPSGameModeBase::ResetOnePlayer(AController* Controller)
{
	if (Controller && Controller->GetPawn())
	{
		Controller->GetPawn()->Reset();
	}
	RestartPlayer(Controller);
	SetPlayerColor(Controller);
}

void ATPSGameModeBase::CreateTeamsInfo()
{
	if (!GetWorld()) return;

	int32 TeamID = 1;
	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const auto Controller = It->Get();
		if (!Controller) continue;

		const auto PlayerState = Cast<ATPSPlayerState>(Controller->PlayerState);
		if (!PlayerState) continue;

		//PlayerState->SetPlayerName(Controller->IsPlayerController() ? "Player" : "Bot");
		if (!Controller->IsPlayerController()) PlayerState->SetPlayerName(TeamID == 1 ? "Bot_1" : "Bot_2");
		PlayerState->SetTeamID(TeamID);
		PlayerState->SetTeamColor(DetermineColorByTeam(TeamID));
		SetPlayerColor(Controller);

		TeamID = TeamID == 1 ? 2 : 1;
	}
}

FLinearColor ATPSGameModeBase::DetermineColorByTeam(int32 TeamID) const
{
	if (TeamID - 1 < GameData.TeamColors.Num())
	{
		return GameData.TeamColors[TeamID - 1];
	}
	UE_LOG(LogTPSGameModeBase, Display, TEXT("No Color for team id: %i, set to default: %s"), TeamID, *GameData.DefaultTeamColor.ToString());
	return GameData.DefaultTeamColor;
}

void ATPSGameModeBase::SetPlayerColor(AController* Controller)
{
	if (!Controller) return;

	const auto Character = Cast<ATPSBaseCharacter>(Controller->GetPawn());
	if (!Character) return;

	const auto PlayerState = Cast<ATPSPlayerState>(Controller->PlayerState);
	if (!PlayerState) return;

	Character->SetPlayerColor(PlayerState->GetTeamColor());
}

void ATPSGameModeBase::LogPlayerInfo()
{
	if (!GetWorld()) return;
	
	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const auto Controller = It->Get();
		if (!Controller) continue;

		const auto PlayerState = Cast<ATPSPlayerState>(Controller->PlayerState);
		if (!PlayerState) continue;

		PlayerState->LogInfo();
	}
}

void ATPSGameModeBase::StartRespawn(AController* Controller)
{	
	const auto RespawnAvailable = RoundCountDown > GameData.MinRoundTimeForRespawn + GameData.RespawnTime;
	if (!RespawnAvailable) return;
	
	const auto RespawnComponent = TPSUtils::GetTPSPlayerComponent<UTPSRespawnComponent>(Controller);
	if (!RespawnComponent) return;

	RespawnComponent->Respawn(GameData.RespawnTime);
}

void ATPSGameModeBase::GameOver()
{
	UE_LOG(LogTPSGameModeBase, Display, TEXT("========== GAME OVER =========="));
	LogPlayerInfo();

	for (auto Pawn : TActorRange<APawn>(GetWorld()))
	{
		if (Pawn)
		{
			Pawn->TurnOff();
			Pawn->DisableInput(nullptr);
		}
	}

	SetMatchState(ETPSMatchState::GameOver);
}

void ATPSGameModeBase::SetMatchState(ETPSMatchState State)
{
	if (MatchState == State) return;

	MatchState = State;
	OnMatchStateChanged.Broadcast(MatchState);
}
