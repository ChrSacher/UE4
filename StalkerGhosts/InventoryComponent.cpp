// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "InventoryComponent.h"


UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = false;
	for (uint8 i = 0; i < uint8(ItemCategory::NUM);i++)
		items.Add(static_cast<ItemCategory>(i),TArray<UItemBase*>());
	
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}


bool UInventoryComponent::addItem(UItemBase* Item)
{
	if (!Item) return true;
	items[Item->type].Add(Item);
	currentWeight += Item->weight * Item->ammount;
	//generate UI stuff here
	return true;
}


bool UInventoryComponent::removeItem(UItemBase* Item, uint8 ammount = -1)
{
	if (!Item) return false;
	if (ammount > 0)
	{
		uint8 place = items[Item->type].Find(Item);
		if (place != INDEX_NONE)
		{
			items[Item->type][place]->ammount -= ammount;
			if (items[Item->type][place]->ammount > 0) return true;
		}
		else
		{
			return false;
		}
	}
	items[Item->type].Remove(Item);
	currentWeight -= Item->weight * Item->ammount;
	return true;

}


UItemBase* UInventoryComponent::splitItem(UItemBase* Item,float ratio)
{
	if (!Item) return NULL;
	if(Item->ammount < 2) return;
	UItemBase* newItem = NewObject<UItemBase>(Item);
	int newAmmount = floor(Item->ammount * ratio);
	newItem->ammount = newAmmount;
	Item->ammount -= newAmmount;
	return newItem;
}



bool UInventoryComponent::isEnoughSpace(UItemBase* Item)
{
	if (!Item) return true;
}

void UInventoryComponent::print()
{
	
}

UItemBase* UInventoryComponent::lookForFirstItem(FString &name)
{
	return NULL;
}
TArray<UItemBase*> UInventoryComponent::lookForItems(FString &name)
{
	
	return TArray<UItemBase*>();
}

void UInventoryComponent::loadUI()
{
	if (mainInventoryTemplate)
	{
		if (!mainInventory)
		{
			mainInventory = CreateWidget<UUserWidget>(GetWorld(), mainInventoryTemplate);
			if (!mainInventory) return;
			mainInventory->AddToViewport();
		}
	}
	categories.Empty();
	if (categoryTemplate)
	{
		for (uint8 i = 0; i < uint8(ItemCategory::NUM); i++)
		{
			UUserWidget* x = CreateWidget<UUserWidget>(GetWorld(), categoryTemplate);
			categories.Add(x);
		}
	}
}