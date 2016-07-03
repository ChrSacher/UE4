// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "MagazineMaster.h"


// Sets default values
AMagazineMaster::AMagazineMaster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMagazineMaster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMagazineMaster::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

