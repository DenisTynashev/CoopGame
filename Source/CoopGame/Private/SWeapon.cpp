// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "IConsoleManager.h"
#include "UObject/ConstructorHelpers.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "CoopGame.h"

static int32 DebugWeapoDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeapoDrawing(
	TEXT("COOP.DebugWeapons"), 
	DebugWeapoDrawing,
	TEXT("Draw debug lines for Weapons"), 
	EConsoleVariableFlags::ECVF_Cheat);

// Sets default values
ASWeapon::ASWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);

	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "BeamEnd";

	//static ConstructorHelpers::FObjectFinder<UBlueprint> WeaponBlueprint(TEXT("Blueprint'/Game/Blueprints/BP_Rifle.BP_Rifle_C''"));
	//if (WeaponBlueprint.Object)
	//{
		//CurrentWeaponBlueprint = (UClass*)WeaponBlueprint.Object->GeneratedClass;		
		//CurrentWeaponBlueprint = (UClass*)WeaponBlueprint.Object;
	//}
}

void ASWeapon::Fire()
{
	AActor* MyOwner = GetOwner();
	if (!MyOwner) { return; }
	FVector OutEyeLocation;
	FRotator OutEyeRotation;
	MyOwner->GetActorEyesViewPoint(OutEyeLocation, OutEyeRotation);

	FVector  ShotDirection = OutEyeRotation.Vector();

	FVector TraceEnd = OutEyeLocation + (ShotDirection * 10000);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(MyOwner);
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnPhysicalMaterial = true;

	//
	FVector TracerEndPoint = TraceEnd;

	FHitResult Hit;
	if (GetWorld()->LineTraceSingleByChannel(Hit, OutEyeLocation, TraceEnd, ECollisionChannel::COLLISION_WEAPON, QueryParams))
	{
		AActor* HitActor = Hit.GetActor();
		UGameplayStatics::ApplyPointDamage(HitActor, 20.0f, ShotDirection, Hit, MyOwner->GetInstigatorController(), this, DamageType);			
		EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
		UParticleSystem* SelectedEffect = DefaultImpactEffect;
		switch (SurfaceType)
		{
			case SURFACE_FLESHDEAFAULT:
				SelectedEffect = FleshImpactEffect;
				break;
			case SURFACE_FLESHVULNERABLE:
				SelectedEffect = FleshImpactEffect;
				break;
		}
		if (SelectedEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
		}
		TraceEnd = Hit.ImpactPoint;
	}
	
	if (DebugWeapoDrawing)
	{
		DrawDebugLine(GetWorld(), OutEyeLocation, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);
	}	
	PlayFireEffects(TraceEnd);
}

void ASWeapon::PlayFireEffects(const FVector& TraceEnd)
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}
	if (TracerEffect)
	{
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
		UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
		if (TracerComp)
		{
			TracerComp->SetVectorParameter(TracerTargetName, TraceEnd);
		}
	}
	APawn* MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner)
	{
		APlayerController* PC = Cast<APlayerController>(MyOwner->GetController());
		if (PC)
		{
			PC->ClientPlayCameraShake(FireCamShake);
		}
	}
}

