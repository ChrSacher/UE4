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

