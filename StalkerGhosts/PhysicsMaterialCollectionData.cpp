// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "PhysicsMaterialCollectionData.h"


// Sets default values for this component's properties
UPhysicsMaterialCollectionData::UPhysicsMaterialCollectionData()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPhysicsMaterialCollectionData::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPhysicsMaterialCollectionData::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

USoundBase* UPhysicsMaterialCollectionData::getPhysicsSound(TEnumAsByte<EPhysicalSurface> type, int32 index)
{
	for (int32 i = 0; i < PhysicsMaterial.Num(); i++)
	{
		if (PhysicsMaterial[i].surfaceType == type)
		{
			if (PhysicsMaterial[i].sounds.sounds.Num() <= 0) break;
			else return PhysicsMaterial[i].sounds.sounds[FMath::RandRange(0, PhysicsMaterial[i].sounds.sounds.Num() - 1)];
		}
	}
	if (defaultMaterial.sounds.sounds.Num() <= 0) return NULL;
	else return  defaultMaterial.sounds.sounds[FMath::RandRange(0, defaultMaterial.sounds.sounds.Num() - 1)];
}

FPhysicsMaterialPenetration& UPhysicsMaterialCollectionData::getPhysicsPenetrationData(TEnumAsByte<EPhysicalSurface> type)
{
	for (int32 i = 0; i < PhysicsMaterial.Num(); i++)
	{
		if (PhysicsMaterial[i].surfaceType == type)
		{
			return PhysicsMaterial[i].penetrationData;
		}
	}
	return  defaultMaterial.penetrationData;
}

UMaterial* UPhysicsMaterialCollectionData::getPhysicsDecal(TEnumAsByte<EPhysicalSurface> type, int32 index)
{
	for (int32 i = 0; i < PhysicsMaterial.Num(); i++)
	{
		if (PhysicsMaterial[i].surfaceType == type)
		{
			if (PhysicsMaterial[i].decals.decals.Num() <= 0) break;
			else return PhysicsMaterial[i].decals.decals[FMath::RandRange(0, PhysicsMaterial[i].decals.decals.Num() - 1)];
		}
	}
	if (defaultMaterial.decals.decals.Num() <= 0) return NULL;
	else return  defaultMaterial.decals.decals[FMath::RandRange(0, defaultMaterial.decals.decals.Num() - 1)];
}

FPhysicsMaterialCollection&  UPhysicsMaterialCollectionData::getCollection(TEnumAsByte<EPhysicalSurface> type)
{
	for (int32 i = 0; i < PhysicsMaterial.Num(); i++)
	{
		if (PhysicsMaterial[i].surfaceType == type)
		{
			return PhysicsMaterial[i];
		}
	}
	return defaultMaterial;
}