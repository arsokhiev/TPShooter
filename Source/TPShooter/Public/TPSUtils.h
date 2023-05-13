#pragma once
#include "TPSPlayerState.h"

class TPSUtils
{
public:
	template <typename T>
	static T* GetTPSPlayerComponent(AActor* PlayerPawn)
	{
		if (!PlayerPawn) return nullptr;
		return Cast<T>(PlayerPawn->GetComponentByClass(T::StaticClass()));
	}

	bool static AreEnemies(const AController* Controller1, const AController* Controller2)
	{
		if (!Controller1 || !Controller2 || Controller1 == Controller2) return false;

		const auto PlayerState1 = Cast<ATPSPlayerState>(Controller1->PlayerState);
		const auto PlayerState2 = Cast<ATPSPlayerState>(Controller2->PlayerState);

		return PlayerState1 && PlayerState2 && PlayerState1->GetTeamID() != PlayerState2->GetTeamID();
	}

	bool static AreTeammates(const AController* Controller1, const AController* Controller2)
	{
		if (!Controller1 || !Controller2 || Controller1 == Controller2) return false;

		const auto PlayerState1 = Cast<ATPSPlayerState>(Controller1->PlayerState);
		const auto PlayerState2 = Cast<ATPSPlayerState>(Controller2->PlayerState);

		return PlayerState1 && PlayerState2 && PlayerState1->GetTeamID() == PlayerState2->GetTeamID();
	}

	static FText TextFromInt(int32 Number) { return FText::FromString(FString::FromInt(Number)); }
};
