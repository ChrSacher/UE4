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
bool UWeaponComponent::reload(UBulletItem* bullet)
{
	
		weapon->currentlyLoadedBullet = bullet;
		return bullet != NULL;
}
void UWeaponComponent::endReload()
{
	isReloading = false;
}

UBulletItem* UWeaponComponent::getLoadedMag()
{
	if (weapon) return weapon->currentlyLoadedBullet;
	return NULL;
};
bool UWeaponComponent::Fire(FVector SpawnLocation, FRotator SpawnRotation)
{
	if (!weapon) return false;
	if (isReloading || !weapon->currentlyLoadedBullet)
	{
		//needs to be in 2 times because first one is a check if variable is NULL
		playEmptySound(SpawnLocation);
		return false;
	}
	bool empty = weapon->currentlyLoadedBullet->ammount <= 0;


	if (!empty)
	{
		playSound(SpawnLocation);
		GetWorld()->SpawnActor<ABullet>(weapon->currentlyLoadedBullet->bullet, SpawnLocation, SpawnRotation);
		weapon->currentlyLoadedBullet->ammount -= 1;
		return true;


	}
	else //if thjere is no bullets in the mag play the 
	{
		playEmptySound(SpawnLocation);
		return false;

	}
}

void UWeaponComponent::loadWeapon(UWeaponItem* wep)
{
	
	if (weapon) weapon->Destroy();

	weapon = GetWorld()->SpawnActor<AWeapon>(wep->wep, FVector(), FRotator());
	if (!weapon) return;
	loadedWeapon = wep->itemIdentifier;
	weapon->weaponID = wep->itemIdentifier;
	
}
int32  UWeaponComponent::getAmmoCount()
{
	if (weapon) if(weapon->currentlyLoadedBullet) return weapon->currentlyLoadedBullet->ammount;
	return 0;
}
void UWeaponComponent::loadWeapon(AWeapon* ID)
{
	if (weapon) weapon->Destroy();
	weapon = ID;
	loadedWeapon = ID->weaponID;
}
void UWeaponComponent::removeWeapon()
{
	if (weapon) weapon->Destroy();
	weapon = NULL;
	loadedWeapon = "";

}

bool UWeaponComponent::loadMag(UBulletItem* ID)
{
	if (!ID) return false;
	UBulletItem* x = DuplicateObject(ID, NULL);
	x->ammount = 0;
	if (weapon)
	{
		if (weapon->ammoCapacity <= ID->ammount)
		{
			x->ammount = weapon->ammoCapacity;
			ID->ammount -= weapon->ammoCapacity;
		}
		else
		{
			x->ammount = ID->ammount;
			ID->ammount = 0;
		}
		weapon->currentlyLoadedBullet = x;
	}
	else return false;
	return true;
}

void UWeaponComponent::unloadMag()
{
	weapon->currentlyLoadedBullet = NULL;
}