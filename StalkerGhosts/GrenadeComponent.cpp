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

	FGrenadeLookUpTable* row = grenadeTable->FindRow<FGrenadeLookUpTable>(FName(*selectedGrenade), ContextString);
	if (!row)
	{
		UE_LOG(LogTemp, Warning, TEXT("GrenadeRowNotFound"));
		return false;
	}
	if (!row->grenade)
	{
		UE_LOG(LogTemp, Warning, TEXT("GrenadeNotFound"));
		return false;
	}
	AGrenade* gren = GetWorld()->SpawnActor<AGrenade>(row->grenade, SpawnLocation, SpawnRotation);
	if (!gren) return false;
	gren->loadGrenadeFromDataTable(row);
	gren->activate();
	return true;
}