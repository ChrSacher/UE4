// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "DataTables.h"
#include "ItemBase.h"
#include "Buff.h"
#include "Weapon.h"
UItemBase::UItemBase()
{
	
}

UItemBase::~UItemBase()
{

}

bool UItemBase::equals(UItemBase* other)
{
	return name == other->name;
}

void UItemBase::loadFromTable(FItemLookUpTable* table)
{

		picture = table->picture;
		weight = table->weight;
		ammount = table->ammount;
		maxAmmount = table->maxAmmount;
		name = table->name;
		groundMesh = table->groundMesh;
		worth = table->worth;
		type= table->type ;
		itemIdentifier = table->itemIdentifier;
}

float UItemBase::getWeight() { return ammount * weight; }

void UBuffHolderItem::initialize(UWorld* world)
{
	attachedBuffs.Empty();
	for (int i = 0; i < buffInit.Num(); i++)
	{
		attachedBuffs.Add(DuplicateObject<UBuff>(Cast<UBuff>(buffInit[i]->GetDefaultObject()), NULL));
	}
}

TArray<UBuff*> UBuffHolderItem::getBuffs()
{
	
	return attachedBuffs;
}

void UWeaponItem::initialize(UWorld* world)
{
	generatedWeapon = world->SpawnActor<AWeapon>(wep, FVector(), FRotator());
	if(generatedWeapon) generatedWeapon->SetActorHiddenInGame(true);
}
