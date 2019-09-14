// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "SWeaponLauncher.generated.h"

class ASProjectile;

/**
 * 
 */
UCLASS()
class COOPGAME_API ASWeaponLauncher : public ASWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASWeaponLauncher();

protected:

	virtual void Fire();

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<ASProjectile> ProjectileClass;
};
