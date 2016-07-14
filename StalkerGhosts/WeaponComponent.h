// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <map>
#include "ItemBase.h"
#include "Bullet.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )

class STALKERGHOSTS_API UWeaponComponent : public UItemBase
{
	
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		

	//std::map<std::string, UMagazineComponent> compatibleMagazines;


	UPROPERTY(EditAnywhere, Category = Weapon)
		float muzzleVelocityCoeff = 1.0f; 

	UPROPERTY(EditAnywhere, Category = Weapon)
		float reloadTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GInventory)
		FTimerHandle reloadHandle;

	UPROPERTY(EditAnywhere, Category = Weapon)
		bool isReloading = false;

	UPROPERTY(EditAnywhere, Category = Weapon)
		float fireRate = 600.0f;

	UPROPERTY(EditAnywhere, Category = Weapon)
		USoundBase*  weaponSound;

	UPROPERTY(EditAnywhere, Category = Weapon)
		USoundBase*  emptySound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
		TSubclassOf<class ABullet> bullet;

	UPROPERTY(EditAnywhere, Category = Magazine)
		int32 ammoCapacity = 30;

	UPROPERTY(EditAnywhere, Category = Magazine)
		int32 currentAmmoCount = 30;
	UPROPERTY(EditAnywhere, Category = Magazine)
		FString currentLoadedBullet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		USkeletalMeshComponent* mesh;

	UPROPERTY(EditAnywhere, Category = Weapon)
		TArray<FString> acceptedBullets;

	UPROPERTY(EditAnywhere, Category = Weapon)
		TSubclassOf<UParticleSystem> flash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GMagazine)
		USpotLightComponent* light;
	/** AnimMontage to play each time we fire */
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* ReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* IdlingAnimation;


	void playSound(FVector place);

	void playEmptySound(FVector place);

	void reload(float bulletammount);
	void endReload();
	bool Fire(FVector SpawnLocation, FRotator SpawnRotation);

};
