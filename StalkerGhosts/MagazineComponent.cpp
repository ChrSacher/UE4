// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "MagazineComponent.h"


// Sets default values for this component's properties
UMagazineComponent::UMagazineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
			// ...
}


// Called when the game starts
void UMagazineComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMagazineComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

bool UMagazineComponent::fireBullet(FVector SpawnLocation, FRotator SpawnRotation)
{
	if (currentAmmoCount <= 0) return false;
	
	GetWorld()->SpawnActor<ABullet>(bullet, SpawnLocation, SpawnRotation);
	currentAmmoCount -= 1;
	return true;
}