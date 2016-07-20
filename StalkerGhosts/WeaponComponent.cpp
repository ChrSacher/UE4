// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "Weapon.h"
#include "WeaponComponent.h"


// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}


// Called every frame
void UWeaponComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UWeaponComponent::playSound(FVector place)
{
	if (weapon->weaponSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, weapon->weaponSound, place);
	}
}

void UWeaponComponent::playEmptySound(FVector place)
{
	if (weapon->emptySound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, weapon->emptySound, place);
	}
}
void UWeaponComponent::startReload()
{
	isReloading = true;
	GetWorld()->GetTimerManager().SetTimer(reloadHandle, this, &UWeaponComponent::endReload, weapon->reloadTime, false);
}
bool UWeaponComponent::reload(int32 &bulletammount)
{
	if (weapon->currentAmmoCount >= weapon->ammoCapacity)
	{
		weapon->currentAmmoCount = weapon->ammoCapacity;
		return true;
	}
	if (bulletammount >= (weapon->ammoCapacity - weapon->currentAmmoCount))
	{
		int32 toAdd = weapon->ammoCapacity - weapon->currentAmmoCount;
		weapon->currentAmmoCount += toAdd;
		bulletammount -= toAdd;

		return true;
	}
	else
	{
		weapon->currentAmmoCount += bulletammount;
		bulletammount = 0;
		return false;
	}
	return false;
	
}
void UWeaponComponent::endReload()
{
	isReloading = false;
}
bool UWeaponComponent::Fire(FVector SpawnLocation, FRotator SpawnRotation)
{
	if (isReloading)
	{
		//needs to be in 2 times because first one is a check if variable is NULL
		playEmptySound(SpawnLocation);
		return false;
	}
	bool empty = weapon->currentAmmoCount <= 0;


	if (!empty)
	{
		playSound(SpawnLocation);
		static const FString ContextString(TEXT("GENERAL"));
		FBulletLookUpTable* row = bulletDataTable->FindRow<FBulletLookUpTable>(FName(*weapon->currentLoadedBullet), ContextString);
		if (!row)
		{
			UE_LOG(LogTemp, Warning, TEXT("BulletRowNotFound"));
			return false;
		}
		if (row->bullet)
		{
			GetWorld()->SpawnActor<ABullet>(row->bullet, SpawnLocation, SpawnRotation)->loadFromDataTable(row);
			weapon->currentAmmoCount -= 1;
			return true;
		}
		return false;

	}
	else //if thjere is no bullets in the mag play the 
	{
		playEmptySound(SpawnLocation);
		return false;

	}
}

void UWeaponComponent::loadWeapon(FString &ID)
{
	
	static const FString ContextString(TEXT("GENERAL"));
	FWeaponLookUpTable* row = weaponDataTable->FindRow<FWeaponLookUpTable>(FName(*ID), ContextString);
	if (!row)
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponRowNotFound"));
		return;
	}
	if (weapon) weapon->Destroy();
	weapon = GetWorld()->SpawnActor<AWeapon>(row->weapon, FVector(), FRotator());
	weapon->loadWeapon(row);
	loadedWeapon = ID;
	weapon->weaponID = ID;
	
}

void UWeaponComponent::loadWeapon(AWeapon* ID)
{
	weapon = ID;
	loadedWeapon = ID->weaponID;
}