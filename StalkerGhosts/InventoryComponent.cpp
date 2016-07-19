// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "InventoryComponent.h"
#include "DataTables.h"
#include "ItemBaseActor.h"
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
		items.Add(static_cast<ItemCategory>(i),TMap<UItemBase*, UItemBase*>());
	
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

bool UInventoryComponent::addItemCreate(UItemBase* Item)
{
	items[Item->type].Add(Item);
	currentWeight += Item->weight * Item->ammount;
	if (Item->widget) Item->widget->RemoveFromParent();
	UItemWidget* x = CreateWidget<UItemWidget>(GetWorld(), itemTemplate);
	Item->widget = x;
	//mainInventory->ItemBox->AddChild(x)->SetSize(FSlateChildSize());

	return true;
}
bool UInventoryComponent::addItem(UItemBase* Item,bool forceNew)
{
	if (!Item) return true;

	if (forceNew)
	{
		addItemCreate(Item);
	}
	else
	{
		if (items.Find(Item->type))
		{
			for (auto& x : items[Item->type])
			{
				if (x.Key->name == Item->name)
				{
					uint8 ammountPossibleToAdd = x.Key->maxAmmount - x.Key->ammount;
					if (ammountPossibleToAdd  < Item->ammount )
					{
						x.Key->ammount += ammountPossibleToAdd;
						Item->ammount -= ammountPossibleToAdd;
					}
					else
					{
						x.Key->ammount += Item->ammount;
						Item->ammount = 0;
					}
				}
			}
			if (Item->ammount > 0)
			{
				addItemCreate(Item);
			}
		}
	}
	refresh();
	return true;
}
UItemBase* UInventoryComponent::addItem(FString& ID, bool forceNew)
{
	UItemBase* base = NewObject<UItemBase>();
	static const FString ContextString(TEXT("GENERAL"));
	FItemLookUpTable* row = itemTable->FindRow<FItemLookUpTable>(FName(*ID), ContextString);
	if (!row)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemRowNotFound"));
		return NULL;
	}
	base->loadFromTable(row);
	if (addItem(base, forceNew)) return base;
	return NULL;
}

bool UInventoryComponent::removeItem(UItemBase* Item, int8 ammount)
{
	if (!Item) return false;
	if (ammount > 0)
	{
		UItemBase* place = items[Item->type][Item];
		if (place)
		{

			if (ammount > place->ammount) ammount = place->ammount;
			currentWeight -= place->weight * ammount;
			place->ammount -= ammount;
			if (place->ammount > 0) return false;
			//fallthrough to past if nothing is left of the thing
		}
		else
		{
			return false;
		}
	}
	dropItem(Item);
	return true;

}

void UInventoryComponent::dropItem(UItemBase* Item)
{
	items[Item->type].Remove(Item);
	if(Item->ammount > 0) GetWorld()->SpawnActor<AItemBaseActor>(itemBaseTemplate, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation())->spawn(Item);
	Item->widget->RemoveFromParent();
}

UItemBase* UInventoryComponent::splitItem(UItemBase* Item,float ratio)
{
	if (!Item) return NULL;
	if(Item->ammount < 2) return Item;
	UItemBase* newItem = NewObject<UItemBase>(Item);
	int newAmmount = floor(Item->ammount * ratio);
	newItem->ammount = newAmmount;
	Item->ammount -= newAmmount;
	addItem(newItem);
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
	for (auto& x : items)
	{
		for (auto& y : x.Value)
		{
			if (y.Value->dataTabelIdentifier == name) return y.Value;
		}
	}
	return NULL;
}
TArray<UItemBase*> UInventoryComponent::lookForItems(FString &name)
{
	TArray<UItemBase*> vec;
	for (auto& x : items)
	{
		for (auto& y : x.Value)
		{
			if (y.Value->dataTabelIdentifier == name) vec.Add(y.Value);
		}
	}
	return vec;
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
				
				return;
			}
			
			mainInventory->AddToViewport();
		}
		
	}
	
	categories.Empty();
	if (categoryTemplate)
	{
		static const FString ContextString(TEXT("GENERAL"));
		for (uint8 i = 0; i < uint8(ItemCategory::NUM); i++)
		{
			UItemCategoryWidget* x = CreateWidget<UItemCategoryWidget>(GetWorld(), categoryTemplate);
			categories.Add(x);
			mainInventory->CategoryBox->AddChildToHorizontalBox(x)->SetSize(FSlateChildSize());
			x->CategoryButton->UserNumber = i;
			x->CategoryButton->CategoryIdentifier = true;
			x->CategoryButton->click.BindUObject(this, &UInventoryComponent::onCategoryClicked);
			FCategoryLookUpTable* row = categoryTable->FindRow<FCategoryLookUpTable>(FName(* FString::FromInt(i)), ContextString);
			if (!row)
			{
				UE_LOG(LogTemp, Warning, TEXT("CategoryRowNotFound"));
			}
			else
			{
				x->CategoryPicture = row->CategoryPicture;
			}
			
		}
	}
}

void UInventoryComponent::refresh()
{
	mainInventory->ItemBox->ClearChildren();
	if (currentCategory == ItemCategory::ALL)
	{
		for (auto& s : items)
		{
			for (auto& w : s.Value)
			{
				if (!w.Key->widget)
				{
					w.Key->widget = CreateWidget<UItemWidget>(GetWorld(), itemTemplate);
				}
				mainInventory->ItemBox->AddChild(w.Key->widget);
				w.Key->widget->ItemButton->CategoryIdentifier = false;
				w.Key->widget->ItemButton->UserPointer = w.Key;
				w.Key->widget->ItemButton->click.Unbind();
				w.Key->widget->ItemButton->click.BindUObject(this, &UInventoryComponent::onItemButtonClicked);
				w.Key->widget->ItemButton->hover.Unbind();
				w.Key->widget->ItemButton->hover.BindUObject(this, &UInventoryComponent::onItemButtonHovered);
				w.Key->widget->ItemButton->unhover.Unbind();
				w.Key->widget->ItemButton->unhover.BindUObject(this, &UInventoryComponent::onItemButtonLeftHovered);
			}
		}
		return;
	}
	
	for (auto& s : items[currentCategory])
	{
		if (!s.Key->widget)
		{
			s.Key->widget = CreateWidget<UItemWidget>(GetWorld(), itemTemplate);
		}
		mainInventory->ItemBox->AddChild(s.Key->widget);
		//mainInventory->ItemBox->
		s.Key->widget->ItemButton->CategoryIdentifier = false;
		s.Key->widget->ItemButton->UserPointer = s.Key;
		s.Key->widget->ItemButton->click.Unbind();
		s.Key->widget->ItemButton->click.BindUObject(this, &UInventoryComponent::onItemButtonClicked);
		s.Key->widget->ItemButton->hover.Unbind();
		s.Key->widget->ItemButton->hover.BindUObject(this, &UInventoryComponent::onItemButtonHovered);
		s.Key->widget->ItemButton->unhover.Unbind();
		s.Key->widget->ItemButton->unhover.BindUObject(this, &UInventoryComponent::onItemButtonLeftHovered);
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
	mainInventory->ItemBox->ClearChildren();
	currentCategory = ItemCategory(sender->UserNumber);
	auto it = items.Find(currentCategory);
	if (it)
	{
		refresh();
	}
	
}
void UInventoryComponent::onItemButtonClicked(UDataItemButton* sender)
{
	selectedItem = Cast<UItemBase>(sender->UserPointer);
}

void UInventoryComponent::onItemButtonHovered(UDataItemButton* sender)
{
	UE_LOG(LogTemp, Warning, TEXT("Ihover"));
	//details menu
}

void UInventoryComponent::onItemButtonLeftHovered(UDataItemButton* sender)
{
	UE_LOG(LogTemp, Warning, TEXT("Iunh"));
}