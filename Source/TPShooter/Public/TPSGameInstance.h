// Third Player Shooter, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TPSCoreTypes.h"
#include "TPSGameInstance.generated.h"

class USoundClass;

UCLASS()
class TPSHOOTER_API UTPSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	FLevelData GetStartupLevel() const { return StartupLevel; }
	void SetStartupLevel(const FLevelData& Data) { StartupLevel = Data; }

	const TArray<FLevelData>& GetLevelsData() const { return LevelsData; }
	
	FName GetMenuLevelName() const { return MenuLevelName; }

	void ToggleVolume();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game")
	TArray<FLevelData> LevelsData;

	UPROPERTY(EditDefaultsOnly, Category = "Game")
	FName MenuLevelName = NAME_None;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundClass* BaseSoundClass;

private:
	FLevelData StartupLevel;
};
