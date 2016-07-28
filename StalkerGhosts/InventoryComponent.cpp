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
#include "InventoryInterface.h"
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
	loadUI();
	
	for (int i = 0; i < beginPlayEquipment.Num(); i++)
	{
		UItemBase* x = Cast<UItemBase>(NewObject<UItemBase>(beginPlayEquipment[i]));
		AWeapon* y = Cast<AWeapon>(x);
		if (beginPlayEquipment[i]) addItem(x);
	}
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
	items[Item->type].Add(Item,Item);
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
template <typename T>
T* UInventoryComponent::createItem(FString ID)
{
	
	static const FString ContextString(TEXT("GENERAL"));
	FItemTable* row = standardItemTable->FindRow<FItemTable>(FName(*ID), ContextString);
	if (!row)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemRowNotFound"));
		return NULL;
	}
	return DuplicateObject(Cast<T>(row->base->GetDefaultObject()), NULL);;
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
	items[Item->type].Remove(Item);
	refresh();
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
	UItemBase* newItem = DuplicateObject(Item,NULL);
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
			if (y.Key->dataTabelIdentifier == name) vec.Add(y.Value);
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
			
			
			mainInventory->AddToPlayerScreen();
			
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
			mainInventory->CategoryBox->AddChildToHorizontalBox(x)->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
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
				x->categoryText = row->categoryText;
			}
			
		}
	}
	
	if (itemDetailTemplate)
	{
		itemDetails = CreateWidget<UItemDetailWidget>(GetWorld(), itemDetailTemplate);
		itemDetails->SetVisibility(ESlateVisibility::Hidden);
	}
	mainInventory->SetVisibility(ESlateVisibility::Hidden);
	refresh();

	


	
	mainInventory->helmetEquipped->onEq.BindUObject(this,&UInventoryComponent::equip);
	mainInventory->armorEquipped->onEq.BindUObject(this, &UInventoryComponent::equip);
	mainInventory->backBackEquipped->onEq.BindUObject(this, &UInventoryComponent::equip);
	mainInventory->weapon1Equipped->onEq.BindUObject(this, &UInventoryComponent::equip);
	mainInventory->weapon1BulletEquipped->onEq.BindUObject(this, &UInventoryComponent::equip);
	mainInventory->artifact1Equipped->onEq.BindUObject(this, &UInventoryComponent::equip);
	mainInventory->artifact2Equipped->onEq.BindUObject(this, &UInventoryComponent::equip);
	mainInventory->artifact3Equipped->onEq.BindUObject(this, &UInventoryComponent::equip);
	mainInventory->artifact4Equipped->onEq.BindUObject(this, &UInventoryComponent::equip);
	mainInventory->quick1Equipped->onEq.BindUObject(this, &UInventoryComponent::equip);
	mainInventory->quick2Equipped->onEq.BindUObject(this, &UInventoryComponent::equip);
	mainInventory->quick3Equipped->onEq.BindUObject(this, &UInventoryComponent::equip);
	mainInventory->quick4Equipped->onEq.BindUObject(this, &UInventoryComponent::equip);

	mainInventory->helmetEquipped->offEq.BindUObject(this, &UInventoryComponent::unEquip);
	mainInventory->armorEquipped->offEq.BindUObject(this, &UInventoryComponent::unEquip);
	mainInventory->backBackEquipped->offEq.BindUObject(this, &UInventoryComponent::unEquip);
	mainInventory->weapon1Equipped->offEq.BindUObject(this, &UInventoryComponent::unEquip);
	mainInventory->weapon1BulletEquipped->offEq.BindUObject(this, &UInventoryComponent::unEquip);
	mainInventory->artifact1Equipped->offEq.BindUObject(this, &UInventoryComponent::unEquip);
	mainInventory->artifact2Equipped->offEq.BindUObject(this, &UInventoryComponent::unEquip);
	mainInventory->artifact3Equipped->offEq.BindUObject(this, &UInventoryComponent::unEquip);
	mainInventory->artifact4Equipped->offEq.BindUObject(this, &UInventoryComponent::unEquip);
	mainInventory->quick1Equipped->offEq.BindUObject(this, &UInventoryComponent::unEquip);
	mainInventory->quick2Equipped->offEq.BindUObject(this, &UInventoryComponent::unEquip);
	mainInventory->quick3Equipped->offEq.BindUObject(this, &UInventoryComponent::unEquip);
	mainInventory->quick4Equipped->offEq.BindUObject(this, &UInventoryComponent::unEquip);


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
		if (s.Key->widget)
		{
			mainInventory->ItemBox->AddChild(s.Key->widget);
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
}


void UInventoryComponent::setVisiblity(bool Vis)
{
	if(Vis) mainInventory->RemoveFromViewport();
	if (!Vis) mainInventory->AddToViewport();
}

void UInventoryComponent::onCategoryClicked(UDataItemButton* sender)
{
	
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
	
	itemDetails->SetVisibility(ESlateVisibility::Visible);
	sender->AddChild(itemDetails);
//	itemDetails->SetPositionInViewport(sender->RenderTransform.Translation);
	//details menu
}

void UInventoryComponent::onItemButtonLeftHovered(UDataItemButton* sender)
{
	UE_LOG(LogTemp, Warning, TEXT("Iunh"));
	sender->RemoveChild(itemDetails);
	itemDetails->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryComponent::equip(UEquippedItemWidget* slot, UItemBase* base)
{
	UE_LOG(LogTemp, Warning, TEXT("EQUIP"));
	if (slot->allowedType != base->type) return;
	
	IInventoryInterface* own = Cast<IInventoryInterface>(GetOwner());
	if (own)
	{
		if(!own->equipmentAdded(base, slot->slotEnum)) return;
	}
	slot->ItemButton->UserPointer = base;
	equippedItems[base->type].Add(base, base);
	items[base->type].Remove(base);
	refresh();
}
void UInventoryComponent::moveEquip(UEquippedItemWidget* slot, UItemBase* base)
{
	
}
void UInventoryComponent::unEquip(UEquippedItemWidget* slot, UItemBase* base)
{
	UE_LOG(LogTemp, Warning, TEXT("UNEQUIP"));
	
	IInventoryInterface* own = Cast<IInventoryInterface>(GetOwner());
	if (own)
	{
		if(!own->equipmentRemoved(base,slot->slotEnum)) return;
	}
	slot->ItemButton->UserPointer = NULL;
	equippedItems[base->type].Remove(base);
	items[base->type].Add(base,base);
	refresh();
}
