// Third Player Shooter, All Rights Reserved

#include "Player/TPSBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ATPSBaseCharacter::ATPSBaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need
	// it.
	PrimaryActorTick.bCanEverTick = true;

	// этот компонент нужен, чтобы камера вращалась не вокруг себя а вокруг этого компонента
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	DefaultSpeed = GetCharacterMovement()->MaxWalkSpeed;
	RunSpeed = DefaultSpeed * 2;
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

	//вызываются каждый фрейм (при нажатии)
	//params: 1. mapping в project settings | 2. указатель на объект функция которого вызывается | 3. ссылка на метод :)
	PlayerInputComponent->BindAxis("MoveForward", this, &ATPSBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPSBaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ATPSBaseCharacter::LookUp);
	PlayerInputComponent->BindAxis("TurnAround", this, &ATPSBaseCharacter::TurnAround);

	//функция Jump реализована в классе Pawn
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATPSBaseCharacter::Jump);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ATPSBaseCharacter::Run);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ATPSBaseCharacter::Walk);
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

void ATPSBaseCharacter::LookUp(float Amount)
{
	//param: значение угла, которое будет добавлено к вращению
	AddControllerPitchInput(Amount);
}

void ATPSBaseCharacter::TurnAround(float Amount)
{
	//изменение вращения пауна (чарактера) через вращение контроллера
	AddControllerYawInput(Amount);
}

void ATPSBaseCharacter::Run()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void ATPSBaseCharacter::Walk()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
}
