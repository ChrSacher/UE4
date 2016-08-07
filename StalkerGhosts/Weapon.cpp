// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "DataTables.h"
#include "Weapon.h"
#include "Bullet.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	flash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Flash"));
	allowedFireModes.Add(WeaponFireMode::AUTO);
	RootComponent = mesh;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AWeapon::reloadMag(UBulletItem* mag)
{
	if (!mag) return;
	if(!currentlyLoadedBullet)
	{
		selectedBulletIndex = acceptedBullets.Find(mag->itemIdentifier);
		if (selectedBulletIndex != INDEX_NONE)
		{
			currentlyLoadedBullet = DuplicateObject(mag, NULL);
			currentlyLoadedBullet->ammount = 0;
		}
		else return;

	}
	if (currentlyLoadedBullet->itemIdentifier == mag->itemIdentifier)
	{
		if (currentlyLoadedBullet->ammount < ammoCapacity)
		{
			int32 toAdd = ammoCapacity - currentlyLoadedBullet->ammount;
			if (mag->ammount >= toAdd)
			{

				mag->ammount -= toAdd;
				currentlyLoadedBullet->ammount += toAdd;
			}
			else
			{
				currentlyLoadedBullet->ammount += mag->ammount;
				mag->ammount = 0;
			}
			return;
		}
	}
}

FString AWeapon::getBulletString()
{
	if (selectedBulletIndex >= 0 && selectedBulletIndex < acceptedBullets.Num()) return acceptedBullets[selectedBulletIndex];
	return "";
}

void AWeapon::playSound(FVector place)
{
	if (weaponSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, weaponSound, place);
	}
}

void AWeapon::playEmptySound(FVector place)
{
	if (emptySound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, emptySound, place);
	}
}

void AWeapon::startReload()
{
	isReloading = true;
	GetWorld()->GetTimerManager().SetTimer(reloadHandle, this, &AWeapon::endReload,reloadTime, false);
}
void  AWeapon::startEquip()
{
	isEquipping = true;
	GetWorld()->GetTimerManager().SetTimer(equippingHandle, this, &AWeapon::endEquip, equipTime, false);
}

void AWeapon::endReload()
{
	isReloading = false;
}
void AWeapon::endEquip()
{
	isEquipping = false;
}


bool AWeapon::reload(UBulletItem* bullet)
{

	currentlyLoadedBullet = bullet;
	return bullet != NULL;
}

UBulletItem* AWeapon::getLoadedMag()
{
	return currentlyLoadedBullet;
};

bool AWeapon::Fire(FVector SpawnLocation, FRotator SpawnRotation)
{
	if (isEquipping) return false;
	if (isReloading || !currentlyLoadedBullet || isEquipping  )
	{
		//needs to be in 2 times because first one is a check if variable is NULL
		playEmptySound(SpawnLocation);
		return false;
	}
	
	bool empty = currentlyLoadedBullet->ammount <= 0;

	if (getFireMode() == WeaponFireMode::SINGLE)
	{
		canEndFire = false;
		if (firedBullets >= 1)
		{
			canEndFire = true;
			firedBullets = 0;
			return false;
		}
		
	}
	if (getFireMode() == WeaponFireMode::BURST)
	{
		canEndFire = false;
		if (firedBullets >= 3)
		{
			canEndFire = true;
			firedBullets = 0;
			return false;
			
		}
	}
	if (empty)
	{
		canEndFire = true;
		playEmptySound(SpawnLocation);
		return false;
	}
	else //if thjere is no bullets in the mag play the 
	{
		
		playSound(SpawnLocation);
		ABullet* x = GetWorld()->SpawnActor<ABullet>(currentlyLoadedBullet->bullet, SpawnLocation, SpawnRotation);
		if (Cast<ACharacter>(GetOwner())) x->controllerOver = Cast<ACharacter>(GetOwner())->GetController();
		currentlyLoadedBullet->ammount -= 1;
		firedBullets += 1;
		return true;
	}
}





int32  AWeapon::getAmmoCount()
{
	if (currentlyLoadedBullet) return currentlyLoadedBullet->ammount;
	return 0;
}

void AWeapon::removeWeapon()
{
	
}

bool AWeapon::loadMag(UBulletItem* ID)
{
	if (!ID) return false;
	UBulletItem* x = DuplicateObject(ID, NULL);
	x->ammount = 0;
	
	if (ammoCapacity <= ID->ammount)
	{
		x->ammount = ammoCapacity;
		ID->ammount -= ammoCapacity;
	}
	else
	{
		x->ammount = ID->ammount;
		ID->ammount = 0;
	}
	currentlyLoadedBullet = x;
	return true;
}

UBulletItem* AWeapon::unloadMag()
{
	UBulletItem* x = currentlyLoadedBullet;
	currentlyLoadedBullet = NULL;
	return x;
}

void AWeapon::switchFireMode()
{
	selectedFireMode++;
	if (selectedFireMode >= allowedFireModes.Num() || selectedFireMode < 0)
	{
		selectedFireMode = 0;
	}
	firedBullets = 0;
}
WeaponFireMode AWeapon::getFireMode()
{
	if (selectedFireMode >= allowedFireModes.Num() && selectedFireMode >= 0)
	{
		return allowedFireModes[selectedFireMode];
	}
	selectedFireMode = 0;
	return WeaponFireMode::SINGLE;
}