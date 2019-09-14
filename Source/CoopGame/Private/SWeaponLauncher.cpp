// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeaponLauncher.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "SProjectile.h"

// Sets default values
ASWeaponLauncher::ASWeaponLauncher()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
}

void ASWeaponLauncher::Fire()
{
	AActor* MyOwner = GetOwner();
	if (!MyOwner) { return; }
	UE_LOG(LogTemp, Warning, TEXT("Grenade firing!"));
	FVector OutEyeLocation;
	FRotator OutEyeRotation;
	MyOwner->GetActorEyesViewPoint(OutEyeLocation, OutEyeRotation);

	FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
	FRotator MuzzleRotation = MeshComp->GetSocketRotation(MuzzleSocketName);
	FActorSpawnParameters SpawnParameters;
	//SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding; -> My variant
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//SpawnParameters.Instigator = MyOwner;
	GetWorld()->SpawnActor<ASProjectile>(ProjectileClass, MuzzleLocation, OutEyeRotation, SpawnParameters);
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}
}