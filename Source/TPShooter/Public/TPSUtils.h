#pragma once

class TPSUtils
{
public:
	template <typename T>
	static T* GetTPSPlayerComponent(AActor* PlayerPawn)
	{
		if (!PlayerPawn) return nullptr;
		return Cast<T>(PlayerPawn->GetComponentByClass(T::StaticClass()));
	}
};