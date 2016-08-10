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
	for (uint8 i = 0; i <(uint8) DamageBodyPart::NUM; i++)
	{
		FDamageModifierStruct x;
		x.type = (DamageBodyPart)i;
		x.modifierValue = 1.0f;
		damageValues.Add(x);
	}
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
	for (int32 i = 0; i < damageValues.Num(); i++)
	{
		
		damageModifierMap.Add(damageValues[i].type, damageValues[i].modifierValue);
	}
	for (int32 i = 0; i < boneBodyParts.Num(); i++)
	{
		for (int32 y = 0; y < boneBodyParts[i].boneNames.Num(); y++)
		{
			boneBodyPartMap.Add(boneBodyParts[i].boneNames[y], boneBodyParts[i].type);
		}
		
	}
	
	for(int32 i = 0; i  < (uint8) DamageBodyPart::NUM;i++) armorValues.Add(1.0f);
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
		return damage * *mod * armorMod;
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