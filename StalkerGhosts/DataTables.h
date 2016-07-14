#pragma once

#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Components/ActorComponent.h"
#include "DataTables.generated.h"

USTRUCT(Blueprintable)
struct FWeaponLookUpTable : public FTableRowBase
{
	GENERATED_BODY()

		/** Full Path of Blueprint */
	UPROPERTY(EditAnywhere, Category = Weapon)
		float muzzleVelocityCoeff = 1.0f;

	UPROPERTY(EditAnywhere, Category = Weapon)
		float reloadTime = 5.0f;


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		USkeletalMeshComponent* mesh;

	UPROPERTY(EditAnywhere, Category = Weapon)
		TSubclassOf<UParticleSystem> flash;

	/** AnimMontage to play each time we fire */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* ReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* IdlingAnimation;

};