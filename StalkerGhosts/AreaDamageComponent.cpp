// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "AreaDamageComponent.h"


// Sets default values for this component's properties
UAreaDamageComponent::UAreaDamageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UAreaDamageComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}


// Called every frame
void UAreaDamageComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UAreaDamageComponent::addActor(AActor* actor)
{
	if (actor) actorsInsideArea.Add(actor);
	if (!isRunning && actorsInsideArea.Num() > 0)
	{

		GetWorld()->GetTimerManager().SetTimer(tickHandle, this, &UAreaDamageComponent::runTick, tickTimeInSeconds, true);
	}
}
void UAreaDamageComponent::runTick()
{
	for (int32 i = 0; i < actorsInsideArea.Num(); i++)
	{
		FMainDamageEvent da;
		FHitResult x;
		da.hit = x;
		da.type = damageType;
		actorsInsideArea[i]->TakeDamage(damage, da, NULL, GetOwner());
	}
}
void UAreaDamageComponent::removeActor(AActor* actor)
{
	if (actor) 
	{
		actorsInsideArea.Remove(actor);
	}
	if (actorsInsideArea.Num() <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(tickHandle);
	}

}