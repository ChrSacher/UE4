// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MagazineComponent.h"
#include <map>

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

	UPROPERTY(EditAnywhere, Category = Weapon)
		USkeletalMeshComponent* mesh;

	UPROPERTY(EditAnywhere, Category = Weapon)
		TArray<FString> acceptedMagazines;

	UPROPERTY(EditAnywhere, Category = Weapon)
		TSubclassOf<UParticleSystem> flash;

	UPROPERTY(EditAnywhere, Category = GMagazine)
		UMagazineComponent* currentMagazine;
	/** AnimMontage to play each time we fire */
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* FireAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* ReloadAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* IdlingAnimation;
	void playSound(FVector place)
	{
		if (weaponSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, weaponSound, place);
		}
	}

	void playEmptySound(FVector place)
	{
		if (emptySound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, emptySound,place);
		}
	}

	void reload(UMagazineComponent* CurrentMagazine)
	{
		currentMagazine = CurrentMagazine;
		isReloading = true;
		GetWorld()->GetTimerManager().SetTimer(reloadHandle, this, &UWeaponComponent::endReload, reloadTime, false);
	}
	void endReload()
	{
		isReloading = false;
	}
	bool Fire(FVector SpawnLocation, FRotator SpawnRotation)
	{
		if (!currentMagazine || isReloading)
		{
			//needs to be in 2 times because first one is a check if variable is NULL
			playEmptySound(SpawnLocation);
			return false;
		}

		if (currentMagazine->fireBullet(SpawnLocation, SpawnRotation) )
		{
			playSound(SpawnLocation);
			
			// try and play a firing animation if specified
			return true;
			
		}
		else //if thjere is no bullets in the mag play the 
		{
			playEmptySound(SpawnLocation);
			return false;

		}
	}

};
