// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "WeaponMaster.h"


// Sets default values
AWeaponMaster::AWeaponMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeaponMaster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponMaster::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

