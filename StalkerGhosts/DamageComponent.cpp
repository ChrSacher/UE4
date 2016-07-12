// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "StalkerGhostsCharacter.h"
#include "DamageComponent.h"


// Sets default values for this component's properties
UDamageComponent::UDamageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UDamageComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDamageComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UDamageComponent::setup()
{
	int maxBones = boneNames.Num();
	if (boneNames.Num() > damageTypes.Num())maxBones = damageTypes.Num();
	for (int i = 0; i < maxBones; i++)
	{
		boneBodyPartMap.Add(boneNames[i], damageTypes[i]);
	}
}

void  UDamageComponent::triggerDamage(DamageBodyPart BodyPart,ABullet* bullet)
{
	AStalkerGhostsCharacter* owner = Cast<AStalkerGhostsCharacter>(GetOwner());
	if(owner && bullet) owner->doDamage(BodyPart,bullet);
	if (owner) owner->doDamage(BodyPart, NULL);
}

void UDamageComponent::checkIfDamage(FString bonename, ABullet* causer)
{
	auto* bone = boneBodyPartMap.Find(bonename);
	if (bone)
	{
		triggerDamage(*bone, causer);
	}
	else
	{
		triggerDamage(DamageBodyPart::NON, causer);
	}
}