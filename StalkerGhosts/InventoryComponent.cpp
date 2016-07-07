// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "InventoryComponent.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

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


bool UInventoryComponent::removeItem(UItemBase* Item, uint8 ammount)
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
	if(Item->ammount < 2) return Item;
	UItemBase* newItem = NewObject<UItemBase>(Item);
	int newAmmount = floor(Item->ammount * ratio);
	newItem->ammount = newAmmount;
	Item->ammount -= newAmmount;
	return newItem;
}



bool UInventoryComponent::isEnoughSpace(UItemBase* Item)
{
	if (!Item) return true;
	return true;
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
			mainInventory = CreateWidget<UMainInventoryWidget>(GetWorld(), mainInventoryTemplate);
			
			if (!mainInventory)
			{
				UE_LOG(LogTemp, Warning, TEXT("Mw"));
				return;
			}
			UE_LOG(LogTemp, Warning, TEXT("Ms"));
			mainInventory->AddToViewport();
		}
		UE_LOG(LogTemp, Warning, TEXT("Md"));
	}
	UE_LOG(LogTemp, Warning, TEXT("that"));
	categories.Empty();
	if (categoryTemplate)
	{
		for (uint8 i = 0; i < uint8(ItemCategory::NUM); i++)
		{
			UItemCategoryWidget* x = CreateWidget<UItemCategoryWidget>(GetWorld(), categoryTemplate);
	
			

			categories.Add(x);
			mainInventory->CategoryBox->AddChildToHorizontalBox(x)->SetSize(FSlateChildSize());
			
			x->CategoryButton->UserNumber = i;
			x->CategoryButton->CategoryIdentifier = true;
			x->CategoryButton->click.BindUObject(this, &UInventoryComponent::onCategoryClicked);
		}
	}
}

void UInventoryComponent::refresh()
{
	mainInventory->ItemBox->ClearChildren();
	auto& s = items[currentCategory];
	for (int i = 0; i < s.Num();i++)
	{
		if (!s[i]->widget)
		{
			s[i]->widget = CreateWidget<UItemWidget>(GetWorld(), itemTemplate);
		}
		s[i]->widget->ItemButton->CategoryIdentifier = false;
		s[i]->widget->ItemButton->UserPointer = s[i];
		s[i]->widget->ItemButton->click.BindUObject(this, &UInventoryComponent::onItemButtonClicked);
		s[i]->widget->ItemButton->hover.BindUObject(this, &UInventoryComponent::onItemButtonHovered);
		s[i]->widget->ItemButton->unhover.BindUObject(this, &UInventoryComponent::onItemButtonLeftHovered);
	} 
}


void UInventoryComponent::setVisiblity(bool Vis)
{
	if(Vis) mainInventory->RemoveFromViewport();
	if (!Vis) mainInventory->AddToViewport();
}

void UInventoryComponent::onCategoryClicked(UDataItemButton* sender)
{
	UE_LOG(LogTemp, Warning, TEXT("cat"));
}
void UInventoryComponent::onItemButtonClicked(UDataItemButton* sender)
{
	UE_LOG(LogTemp, Warning, TEXT("ICLICK"));
}

void UInventoryComponent::onItemButtonHovered(UDataItemButton* sender)
{
	UE_LOG(LogTemp, Warning, TEXT("Ihover"));
}

void UInventoryComponent::onItemButtonLeftHovered(UDataItemButton* sender)
{
	UE_LOG(LogTemp, Warning, TEXT("Iunh"));
}