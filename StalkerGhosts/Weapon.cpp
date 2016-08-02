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