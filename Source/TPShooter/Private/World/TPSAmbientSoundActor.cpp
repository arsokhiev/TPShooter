// Third Player Shooter, All Rights Reserved


#include "World/TPSAmbientSoundActor.h"
#include "Components/AudioComponent.h"

ATPSAmbientSoundActor::ATPSAmbientSoundActor()
{
	PrimaryActorTick.bCanEverTick = false;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
}

void ATPSAmbientSoundActor::BeginPlay()
{
	Super::BeginPlay();

	AudioComponent->FadeIn(10.0f);
}