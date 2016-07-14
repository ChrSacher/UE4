// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "StalkerGhostsCharacter.h"
#include "DamageInterface.h"
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
	for (uint8 i = 0; i < uint8 (DamageBodyPart::NUM); i++)
	{
		damageModifierMap.Add(DamageBodyPart(i), damageModifiers[i]);
	}
}

float  UDamageComponent::damageAmmount(DamageBodyPart BodyPart,ABullet* bullet)
{
	return damageAmmount(BodyPart, bullet->damage);
	
}
float UDamageComponent::damageAmmount(DamageBodyPart BodyPart, float damage)
{
	auto* mod = damageModifierMap.Find(BodyPart);
	if (mod) return damage * *mod;
	return damage;
}
DamageBodyPart  UDamageComponent::getDamagedBodyPart(FString bonename)
{
	auto* bone = boneBodyPartMap.Find(bonename);

	if (bone)
	{
		return *bone;
	}
	else
	{
		return DamageBodyPart::NON;
	}
}
