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
	for (int32 i = 0; i < PhysicsMaterialSounds.Num(); i++)
	{
		if (type == PhysicsMaterialSounds[i].surfaceType)
		{
			if (PhysicsMaterialSounds[i].sounds.Num() == 0) return NULL;
			if (index <= -1)
			{

				return PhysicsMaterialSounds[i].sounds[FMath::RandRange(0, PhysicsMaterialSounds[i].sounds.Num() - 1)];
			}
			else
			{
				if (index < PhysicsMaterialSounds[i].sounds.Num())
				{
					return PhysicsMaterialSounds[i].sounds[index];
				}
				else
				{
					return PhysicsMaterialSounds[i].sounds.Last();
				}
			}
		}
	}
	return NULL;
}

FPhysicsMaterialPenetration& UPhysicsMaterialCollectionData::getPhysicsPenetrationData(TEnumAsByte<EPhysicalSurface> type)
{
	for (int32 i = 0; i < penetrationData.Num(); i++)
	{
		if (type == penetrationData[i].surfaceType)
		{
			return penetrationData[i];
		}
	}
	return defaultPenetrationData;
}