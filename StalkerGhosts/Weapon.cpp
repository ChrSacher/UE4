// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "DataTables.h"
#include "Weapon.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
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

void AWeapon::loadWeapon(FWeaponLookUpTable* row)
{
	muzzleVelocityCoeff = row->muzzleVelocityCoeff;
	reloadTime = row->reloadTime;
	fireRate = row->fireRate;
	weaponSound = row->weaponSound;
	emptySound = row->emptySound;
	acceptedBullets = row->acceptedBullets;
	ammoCapacity = row->ammoCapacity;
	mesh->SetSkeletalMesh(row->mesh);
	flash = row->flash;
	FireAnimation = row->FireAnimation;
	ReloadAnimation = row->ReloadAnimation;
	IdlingAnimation = row->IdlingAnimation;
}