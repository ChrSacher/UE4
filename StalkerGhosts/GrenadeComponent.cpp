// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "GrenadeComponent.h"
#include "DataTables.h"

// Sets default values for this component's properties
UGrenadeComponent::UGrenadeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrenadeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrenadeComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

bool UGrenadeComponent::throwGrenade(FVector SpawnLocation, FRotator SpawnRotation)
{
	
	
	static const FString ContextString(TEXT("GENERAL"));
	if (ableToThrow)
	{
		ableToThrow = false;
		GetWorld()->GetTimerManager().SetTimer(speedHandle, this, &UGrenadeComponent::endThrow, grenadeReloadTime, true);
	}
	else
	{

		return false;
	}

	AGrenade* gren = GetWorld()->SpawnActor<AGrenade>(selectedGrenade, SpawnLocation, SpawnRotation);
	gren->ProjectileMovement->InitialSpeed = grenadeThrowVelocity;
	gren->ProjectileMovement->MaxSpeed = grenadeThrowVelocity;
	if (!gren) return false;
	gren->activate();
	return true;
}

void UGrenadeComponent::endThrow()
{
	ableToThrow = true;
}