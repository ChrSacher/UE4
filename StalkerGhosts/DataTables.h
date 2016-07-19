#pragma once

#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Components/ActorComponent.h"
#include "DamageEnum.h"
#include "Bullet.h"
#include "Grenade.h"
#include "Weapon.h"
#include "ItemEnums.h"
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
		float fireRate = 600.0f;

	UPROPERTY(EditAnywhere, Category = Weapon)
		USoundBase*  weaponSound;

	UPROPERTY(EditAnywhere, Category = Weapon)
		USoundBase*  emptySound;

	UPROPERTY(EditAnywhere, Category = Magazine)
		int32 ammoCapacity = 30;
	UPROPERTY(EditAnywhere, Category = Magazine)
		USpotLightComponent* spotLight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		USkeletalMesh* mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		TArray<FString> acceptedBullets;

	UPROPERTY(EditAnywhere, Category = Weapon)
		TSubclassOf<UParticleSystem> flash;

	/** AnimMontage to play each time we fire */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	class UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	class UAnimMontage* ReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	class UAnimMontage* IdlingAnimation;


	UPROPERTY(EditAnywhere, Category = Weapon)
		TSubclassOf<class AWeapon> weapon;

};

USTRUCT(Blueprintable)
struct FGrenadeLookUpTable : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = Grenade)
		float fuzeTime = 5.0f;

	UPROPERTY(EditAnywhere, Category = Grenade)
		uint32 sharpnelAmmount = 10;

	UPROPERTY(EditAnywhere, Category = Grenade)
		float damage = 10;

	UPROPERTY(EditAnywhere, Category = Grenade)
		float shrapnelDamage = 10;

	UPROPERTY(EditAnywhere, Category = Grenade)
		float range = 150;

	UPROPERTY(EditAnywhere, Category = Grenade)
		float shrapnelVelocitry = 1000;

	UPROPERTY(EditAnywhere, Category = Grenade)
		UParticleSystem* flash;

	UPROPERTY(EditAnywhere, Category = Grenade)
		UStaticMesh* mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Grenade)
		USoundBase* sound;

	UPROPERTY(EditAnywhere, Category = Bullet)
		TSubclassOf<class AGrenade> grenade;
};

USTRUCT(Blueprintable)
struct FBulletLookUpTable : public FTableRowBase
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, Category = Bullet)
		float damage = 10.0f;

	UPROPERTY(EditAnywhere, Category = Bullet)
		float velocity = 7000.0f; //in cm/s
	UPROPERTY(EditAnywhere, Category = Bullet)
		EDamageType type;
	UPROPERTY(EditAnywhere, Category = Bullet)
		UStaticMesh* mesh;

	UPROPERTY(EditAnywhere, Category = Bullet)
		TSubclassOf<class ABullet> bullet;
};

USTRUCT(Blueprintable)
struct FCategoryLookUpTable : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = Bullet)
		UTexture2D* CategoryPicture;
};

USTRUCT(Blueprintable)
struct FItemLookUpTable : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = Item)
		UTexture2D* picture;

	UPROPERTY(EditAnywhere, Category = Item)
		float weight = 1;

	UPROPERTY(EditAnywhere, Category = Item)
		uint16 ammount = 1;

	UPROPERTY(EditAnywhere, Category = Item)
		uint16 maxAmmount = 1;

	UPROPERTY(EditAnywhere, Category = Item)
		FString name = "Default";

	UPROPERTY(EditAnywhere, Category = Item)
		UStaticMesh* groundMesh;

	UPROPERTY(EditAnywhere, Category = Item)
		float worth = 50;

	UPROPERTY(EditAnywhere, Category = Item)
		ItemCategory type = ItemCategory::ITEM;

	UPROPERTY(EditAnywhere, Category = Item)
		FString itemIdentifier;

	UPROPERTY(EditAnywhere, Category = Item)
		FString dataTabelIdentifier;
};