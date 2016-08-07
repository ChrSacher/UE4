// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "DataTables.h"
#include "ItemBase.h"
#include "Buff.h"
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

TArray<UBuff*> UBuffHolderItem::getBuffs()
{
	if (!init)
	{
		for (int i = 0; i < buffInit.Num(); i++)
		{
			attachedBuffs.Add(DuplicateObject<UBuff>(Cast<UBuff>(buffInit[i]->GetDefaultObject()), NULL));
		}
		init = true;
	}
	return attachedBuffs;
}