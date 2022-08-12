// Third Player Shooter, All Rights Reserved

#include "Player/TPSBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"

// Sets default values
ATPSBaseCharacter::ATPSBaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need
	// it.
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ATPSBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATPSBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATPSBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//вызываются каждый фрейм
	//params: 1. mapping в project settings | 2. указатель на объект функция которого вызывается | 3. ссылка на метод :)
	PlayerInputComponent->BindAxis("MoveForward", this, &ATPSBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPSBaseCharacter::MoveRight);
}

void ATPSBaseCharacter::MoveForward(float Amount)
{
	// params: 1. Смещать относительно направления в данный момент | 2. число указанное в mapping | 3. 
	AddMovementInput(GetActorForwardVector(), Amount);
}

void ATPSBaseCharacter::MoveRight(float Amount)
{
	AddMovementInput(GetActorRightVector(), Amount);
}
