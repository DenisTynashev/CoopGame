// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamagableObject.generated.h"

class USHealthComponent;
class URadialForceComponent;
class UMaterialInterface;

UCLASS()
class COOPGAME_API ADamagableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamagableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USHealthComponent* HealthComp;

	UFUNCTION()
	void OnHealthChanged(USHealthComponent* ChangedHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	/*already destroyed*/
	UPROPERTY(ReplicatedUsing=OnRep_Exploded)
    //UPROPERTY(BlueprintReadOnly, Category = "Player")
	bool bDestroyed;

	UFUNCTION()
	void OnRep_Exploded();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URadialForceComponent* ExplosionComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	float ExplosionImpulseStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	float VectorImpulseZ;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Explosion")
	UParticleSystem* ExplosionEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
	UMaterialInterface* MaterialAfterExplosion;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
