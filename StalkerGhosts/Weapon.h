// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Bullet.h"
#include "Weapon.generated.h"

struct FWeaponLookUpTable;
class ABullet;
class UBulletItem;



UENUM(BlueprintType)
enum class WeaponFireMode : uint8
{
	SINGLE,
	BURST,
	AUTO
};

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
		float reloadTime = 2.0f;

	UPROPERTY(EditAnywhere, Category = Weapon)
		float fireRate = 600.0f;

	UPROPERTY(EditAnywhere, Category = Weapon)
		float equipTime = 2.0f;

	UPROPERTY(EditAnywhere, Category = Weapon)
		bool canEndFire = true;

	UPROPERTY(EditAnywhere, Category = Weapon)
		int32 selectedFireMode = 0;

	UPROPERTY(EditAnywhere, Category = Weapon)
		TArray<WeaponFireMode> allowedFireModes;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
		int32 firedBullets = 0;

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
		class UAnimMontage* equipAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* ReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* IdlingAnimation;

	UPROPERTY(EditAnywhere, Category = Weapon)
		TArray<FString> acceptedBullets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GInventory)
		FTimerHandle reloadHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GInventory)
		FTimerHandle equippingHandle;

	UPROPERTY(EditAnywhere, Category = Weapon)
		bool isReloading = false;

	UPROPERTY(EditAnywhere, Category = Weapon)
		bool isEquipping = false;

	UPROPERTY(EditAnywhere, Category = Weapon)
		int32 selectedBulletIndex;

	void reloadMag(UBulletItem* mag);
	FString getBulletString();

	void playEmptySound(FVector place);
	void playSound(FVector place);
	void startReload();
	void startEquip();
	void endReload();
	void endEquip();
	void switchFireMode();
	WeaponFireMode getFireMode();
	bool loadWeapon(AWeapon* ID);
	bool loadMag(UBulletItem* ID);
	UBulletItem* unloadMag();

	int32 getAmmoCount();


	void removeWeapon();
	bool reload(UBulletItem* bullet);

	bool Fire(FVector SpawnLocation, FRotator SpawnRotation);
	UBulletItem* getLoadedMag();
};
