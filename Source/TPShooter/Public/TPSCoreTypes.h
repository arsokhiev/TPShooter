#pragma once

//this is necessary for the Unreal Header Tool to correctly generate code that relates to structure and delegate macros
#include "TPSCoreTypes.generated.h"

//weapon component
class ATPSBaseWeapon;

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ATPSBaseWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	UAnimMontage* ReloadAnimMontage;
};

//weapon
DECLARE_MULTICAST_DELEGATE_OneParam(FOnClipEmptySignature, ATPSBaseWeapon*);
DECLARE_MULTICAST_DELEGATE(FOnShootMade);

USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Bullets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!Infinite"))
	int32 Clips;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	bool Infinite;
};

//weapon UI
USTRUCT(BlueprintType)
struct FWeaponUIData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	UTexture2D* MainIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	UTexture2D* CrossHairIcon;
};

//health component
DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, float);

//VFX
class UNiagaraSystem;
class USoundCue;

USTRUCT(BlueprintType)
struct FDecalData
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UMaterialInterface* Material;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	FVector Size = FVector(10.0f);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	float LifeTime = 5.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	float FadeOutTime = 3.0f;
};

USTRUCT(BlueprintType)
struct FImpactData
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* NiagaraEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	FDecalData DecalData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	USoundCue* Sound;
};

//GameMode
USTRUCT(BlueprintType)
struct FGameData
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "50"))
	int32 PlayersNum = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "1", ClampMax = "10"))
	int32 RoundsNum = 4;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "3", ClampMax = "300"))
	int32 RoundTime = 10; //in seconds

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	FLinearColor DefaultTeamColor = FLinearColor::Red;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	TArray<FLinearColor> TeamColors;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "3", ClampMax = "20"))
	int32 RespawnTime = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game", meta = (ClampMin = "3", ClampMax = "20"))
	int32 MinRoundTimeForRespawn = 10;
};

//GameState
UENUM(BlueprintType)
enum class ETPSMatchState : uint8
{
	WaitingToStart = 0,
	InProgress,
	Pause,
	GameOver
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchStateChangedSignature, ETPSMatchState);

//Levels
USTRUCT(BlueprintType)
struct FLevelData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	FName LevelName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	FName LevelDisplayName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	UTexture2D* LevelThumb;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelSelectedSignature, const FLevelData&);
