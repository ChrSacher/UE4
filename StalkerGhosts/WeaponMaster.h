// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MagazineMaster.h"
#include <map>
#include "GameFramework/Actor.h"
#include "WeaponMaster.generated.h"

UCLASS()
class STALKERGHOSTS_API AWeaponMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponMaster();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, Category = AMMO)
		AMagazineMaster* currentMagazine;

	std::map<std::string, AMagazineMaster> compatibleMagazines;

	UPROPERTY(EditAnywhere, Category = Weapon)
		FString name = FString("Default Weapon");

	UPROPERTY(EditAnywhere, Category = Weapon)
		float muzzleVelocityCoeff = 1.0f;

	UPROPERTY(EditAnywhere, Category = Weapon)
		float reloadTime = 5.0f;

	UPROPERTY(EditAnywhere, Category = Weapon)
		float fireRate = 600.0f;

	UPROPERTY(EditAnywhere, Category = Weapon)
		USoundBase*  weaponSound;

	UPROPERTY(EditAnywhere, Category = Weapon)
		USkeletalMeshComponent* mesh;
	
};
