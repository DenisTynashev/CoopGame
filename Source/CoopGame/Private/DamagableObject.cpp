// Fill out your copyright notice in the Description page of Project Settings.


#include "DamagableObject.h"
#include "..\Public\Components\SHealthComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ADamagableObject::ADamagableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealtComp"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	ExplosionComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("ExplosionComp"));
	ExplosionComp->AddCollisionChannelToAffect(ECollisionChannel::ECC_PhysicsBody);
	Radius = 1000.0f;
	ImpulseStrength = 30000.0f;
	bDestroyed = false;

}

// Called when the game starts or when spawned
void ADamagableObject::BeginPlay()
{
	Super::BeginPlay();
	ExplosionComp->Radius = Radius;
	ExplosionComp->ImpulseStrength = ImpulseStrength;
	ExplosionComp->ForceStrength = 0.0f;
	ExplosionComp->bImpulseVelChange = false;
	ExplosionComp->bIgnoreOwningActor = true;

	HealthComp->OnHealthChanged.AddDynamic(this, &ADamagableObject::OnHealthChanged);
	
}

void ADamagableObject::OnHealthChanged(USHealthComponent* ChangedHealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !bDestroyed)
	{
		//destroy
		bDestroyed = true;
		UE_LOG(LogTemp, Warning, TEXT("Destroyed"));
		ExplosionComp->FireImpulse();
		if (ExplosionEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation());
		}
		if (MaterialAfterExplosion)
		{
			Mesh->SetMaterial(0, MaterialAfterExplosion);
		}
		FVector Upwards = { 0.0f,0.0f,500.0f };
		//UPrimitiveComponent* Barrel = Cast<UPrimitiveComponent>(Mesh);
		Mesh->AddImpulse(Upwards, FName ("NONE"), true);
		
	}
}

// Called every frame
void ADamagableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

