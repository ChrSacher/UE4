// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "DataTables.h"
#include "ItemBase.h"

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
		dataTabelIdentifier = table->dataTabelIdentifier;
		itemIdentifier = table->itemIdentifier;
}