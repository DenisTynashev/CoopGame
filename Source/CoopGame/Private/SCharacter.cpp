// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "..\Public\SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Engine/World.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SpringArmComp->bUsePawnControlRotation = true;
	CameraComp->AttachToComponent(SpringArmComp, FAttachmentTransformRules::KeepRelativeTransform);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = true;
	GetMovementComponent()->SetJumpAllowed(true);
	//Spawn default weapon
	//FVector NewLocation = GetActorLocation();
	//CurrentWeapon = GetWorld()->SpawnActor<ASWeapon>(GetClass(), NewLocation, FRotator::ZeroRotator);
	//CurrentWeapon

}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void ASCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void ASCharacter::BeginCrouch()
{
	Crouch();
}

void ASCharacter::EndCrouch()
{
	UnCrouch();
}

void ASCharacter::DoJump()
{
	Jump();
}

void ASCharacter::ChangeWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Change Weapon!"));
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ASCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ASCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Pressed, this, &ASCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", EInputEvent::IE_Released, this, &ASCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ASCharacter::DoJump);

	PlayerInputComponent->BindAction("ChangeWeapon", EInputEvent::IE_Pressed, this, &ASCharacter::ChangeWeapon);
}

FVector ASCharacter::GetPawnViewLocation() const
{
	if(CameraComp)
	{
		return CameraComp->GetComponentLocation();
	}
	
	return Super::GetPawnViewLocation();
}

