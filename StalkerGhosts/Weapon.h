// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "Weapon.generated.h"

struct FWeaponLookUpTable;
class ABullet;
class UBulletItem;
UCLASS()
class STALKERGHOSTS_API AWeapon :  public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	UPROPERTY(EditAnywhere, Category = Weapon)
		UParticleSystemComponent* flash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		USkeletalMeshComponent* mesh;

	UPROPERTY(EditAnywhere, Category = Magazine)
		int32 ammoCapacity = 30;

	UPROPERTY(EditAnywhere, Category = Magazine)
		UBulletItem* currentlyLoadedBullet;

	UPROPERTY(EditAnywhere, Category = Weapon)
		float muzzleVelocityCoeff = 1.0f;

	UPROPERTY(EditAnywhere, Category = Weapon)
		float reloadTime = 5.0f;

	UPROPERTY(EditAnywhere, Category = Weapon)
		float fireRate = 600.0f;

	UPROPERTY(EditAnywhere, Category = Weapon)
		USoundBase*  weaponSound;

	UPROPERTY(EditAnywhere, Category = Weapon)
		USoundBase*  emptySound;

	UPROPERTY(EditAnywhere, Category = Weapon)
		FString weaponID = "";
	/** AnimMontage to play each time we fire */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* ReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* IdlingAnimation;

	UPROPERTY(EditAnywhere, Category = Weapon)
		TArray<FString> acceptedBullets;

	UPROPERTY(EditAnywhere, Category = Weapon)
		int32 selectedBulletIndex;

	void reloadMag(UBulletItem* mag);
	FString getBulletString();
};
