// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "StalkerGhostsCharacter.h"
#include "DamageInterface.h"
#include "DamageComponent.h"
#include "ItemBase.h"

// Sets default values for this component's properties
UDamageComponent::UDamageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	// ...
	for (int i = 0; i < (uint8)DamageBodyPart::NUM; i++)
	{
		damageValues.Add(FDamageStruct());
		
	};
}


// Called when the game starts
void UDamageComponent::BeginPlay()
{
	Super::BeginPlay();
	for (int i = 0; i < damageValues.Num(); i++)
	{
		boneBodyPartMap.Add(damageValues[i].boneName, damageValues[i].type);
		damageModifierMap.Add(damageValues[i].type, damageValues[i].modifierValue);
	}
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
}

float  UDamageComponent::damageAmmount(DamageBodyPart BodyPart,ABullet* bullet)
{
	return damageAmmount(BodyPart, bullet->damage);
	

}
float UDamageComponent::damageAmmount(DamageBodyPart BodyPart, float damage)
{
	auto* mod = damageModifierMap.Find(BodyPart);
	float armor = armorValues[(uint8)BodyPart];
	if (mod)
	{
		float armorMod = 1;
		if (armor > 0)
		{
			armorMod = 100 / (100 + armor);
		}
		else
		{
			armorMod = 2 - 100 / (100 - armor);
		}
		return damage * *mod + armorMod;
	}
	return damage;;
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

float UDamageComponent::calculateGrenadeDamage(AGrenade* grenade)
{
	if (FVector::Dist(GetOwner()->GetActorLocation(),grenade->GetActorLocation()) > grenade->range / 2)
	{
		
		return (0.25 + 0.75 * FVector::Dist(GetOwner()->GetActorLocation(), grenade->GetActorLocation()) / grenade->range) * grenade->damage;
	}
	else
	{
		return grenade->damage;
	}
}



void UDamageComponent::addArmor(UArmorItem* armor)
{
	for (int i = 0; i < armor->affectedParts.Num(); i++)
	{
		armorValues[(uint8)armor->affectedParts[i]] += armor->ArmorValue;
	}
}
void UDamageComponent::removeArmor(UArmorItem* armor)
{
	if (!armor) return;
	for (int i = 0; i < armor->affectedParts.Num(); i++)
	{
		armorValues[(uint8)armor->affectedParts[i]] -= armor->ArmorValue;
	}
}