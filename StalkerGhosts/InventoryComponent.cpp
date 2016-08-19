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
#include "Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h"
#include "InventoryInterface.h"
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = false;
	for (uint8 i = 0; i < uint8(ItemCategory::NUM);i++)
		items.Add(static_cast<ItemCategory>(i),TMap<UItemBase*, UItemBase*>());

	equipment = CreateDefaultSubobject<UCharacterEquipment>(TEXT("equipment"));
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	
	for (int i = 0; i < beginPlayEquipment.Num(); i++)
	{

		UItemBase* x = Cast<UItemBase>(DuplicateObject(Cast<UItemBase>(beginPlayEquipment[i].item->GetDefaultObject()),NULL));
		if (x)
		{
			x->ammount = beginPlayEquipment[i].ammount;
			x->initialize(GetOwner()->GetWorld());
			addItem(x);
		}
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
	items[Item->type].Add(Item, Item);
	Item->itemParent = this;
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
	
	if (items.Find(Item->type) == NULL) return false;
	if (items[Item->type].Find(Item) != NULL)
	{
		return true;
	}
	if (forceNew)
	{
		addItemCreate(Item);
	}
	else
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
				x.Key->itemParent = this;
			}
			if (Item->ammount > 0)
			{
				addItemCreate(Item);
			}
		
	}
	Item->itemParent = this;
	calculateWeight();
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
	calculateWeight();
	T* x = DuplicateObject(Cast<T>(row->base->GetDefaultObject()), NULL);
	Cast<UItemBase>(x)->initialize(GetOwner()->GetWorld());
	return x;
}
bool UInventoryComponent::removeItem(UItemBase* Item, int32 ammount)
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
	calculateWeight();
	return true;

}

void UInventoryComponent::dropItem(UItemBase* Item)
{
	items[Item->type].Remove(Item);
	if(Item->ammount > 0) GetWorld()->SpawnActor<AItemBaseActor>(itemBaseTemplate, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation())->spawn(Item);
	Item->widget->RemoveFromParent();
	calculateWeight();
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
			if (y.Value->itemIdentifier == name) return y.Value;
		}
	}
	return NULL;
}
TArray<UItemBase*> UInventoryComponent::lookForItems(FString name)
{
	TArray<UItemBase*> vec;
	for (auto& x : items)
	{
		for (auto& y : x.Value)
		{
			if (y.Key->itemIdentifier == name) vec.Add(y.Value);
		}
	}
	return vec;
}
void UInventoryComponent::loadCategories()
{
	if (!mainInventory) return;
	
		static const FString ContextString(TEXT("GENERAL"));
		for (uint8 i = 0; i < mainInventory->categories.Num(); i++)
		{
			if (!mainInventory->categories[i]) continue;
			mainInventory->categories[i]->CategoryButton->UserNumber = mainInventory->otherCategories[i]->cat;
			mainInventory->categories[i]->CategoryButton->CategoryIdentifier = true;
			mainInventory->categories[i]->CategoryButton->click.BindUObject(this, &UInventoryComponent::onCategoryClicked);

		}
		for (uint8 i = 0; i < mainInventory->otherCategories.Num(); i++)
		{
			if (!mainInventory->otherCategories[i]) continue;
			mainInventory->otherCategories[i]->CategoryButton->UserNumber = mainInventory->otherCategories[i]->cat;
			mainInventory->otherCategories[i]->CategoryButton->CategoryIdentifier = true;
			mainInventory->otherCategories[i]->CategoryButton->click.BindUObject(this, &UInventoryComponent::onOtherCategoryClicked);

		}
	
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
	loadCategories();
	
	if (itemDetailTemplate)
	{
		itemDetails = CreateWidget<UItemDetailWidget>(GetWorld(), itemDetailTemplate);
		itemDetails->SetVisibility(ESlateVisibility::Hidden);
	}
	
	mainInventory->ItemBoxWidget->inventoryParent = this;
	mainInventory->SetVisibility(ESlateVisibility::Hidden);

	refresh();

	


	for (uint8 i = 0; i < (uint8)SlotInformation::NUM; i++)
	{
		mainInventory->widgets[i]->inventoryParent = this;
	}

}
void UInventoryComponent::loadItemWidget(UItemBase* item,UScrollBox* scrollBox)
{
	if (!item) return;
	if (!item->widget)
	{
		item->widget = CreateWidget<UItemWidget>(GetWorld(), itemTemplate);
		if (!item->widget) return;
	}
	
	scrollBox->AddChild(item->widget);
	item->widget->ItemButton->CategoryIdentifier = false;
	item->widget->ItemButton->UserPointer = item;
	item->widget->ItemButton->click.Unbind();
	item->widget->ItemButton->click.BindUObject(this, &UInventoryComponent::onItemButtonClicked);
	item->widget->ItemButton->hover.Unbind();
	item->widget->ItemButton->hover.BindUObject(this, &UInventoryComponent::onItemButtonHovered);
	item->widget->ItemButton->unhover.Unbind();
	item->widget->ItemButton->unhover.BindUObject(this, &UInventoryComponent::onItemButtonLeftHovered);
}
void UInventoryComponent::refresh()
{
	if(mainInventory)	mainInventory->ItemBox->ClearChildren();
	for (auto& s : items)
	{
		if (s.Key == currentCategory || currentCategory == ItemCategory::ALL)
		{
			for (auto& w : s.Value)
			{

				loadItemWidget(w.Key, mainInventory->ItemBox);

			}
		}
		
	}
	
	if (otherInventoryForTransfering)
	{
		if (mainInventory)	mainInventory->otherItemBox->ClearChildren();
		for (auto& s : otherInventoryForTransfering->items)
		{
			if (s.Key == otherCurrentCategory || otherCurrentCategory == ItemCategory::ALL)
			{
				for (auto& w : s.Value)
				{

					loadItemWidget(w.Key, mainInventory->otherItemBox);

				}
			}

		}
	}
}


void UInventoryComponent::setVisiblity(bool Vis)
{
	if(Vis) mainInventory->RemoveFromViewport();
	if (!Vis)
	{
		mainInventory->AddToViewport();
		closeTransferWindow();
	}
}

void UInventoryComponent::showInventory()
{
	if(!mainInventory) loadUI();
	if (mainInventory) mainInventory->SetVisibility(ESlateVisibility::Visible);
	
}
bool UInventoryComponent::isInRangeOfOtherInventory(UInventoryComponent* otherInventory)
{
	if (!otherInventory) return false;
	return FVector::Dist(GetOwner()->GetActorLocation(), otherInventory->GetOwner()->GetActorLocation()) < 600;
}
void UInventoryComponent::checkRangeOnOtherInventory()
{
	if (!isInRangeOfOtherInventory(otherInventoryForTransfering)) hideInventory();
}
void UInventoryComponent::hideInventory()
{
	if (mainInventory) mainInventory->SetVisibility(ESlateVisibility::Hidden);
	closeTransferWindow();
	GetWorld()->GetTimerManager().ClearTimer(transferRangeTimer);
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
void UInventoryComponent::onOtherCategoryClicked(UDataItemButton* sender)
{
	mainInventory->otherItemBox->ClearChildren();
	otherCurrentCategory = ItemCategory(sender->UserNumber);
	auto it = items.Find(otherCurrentCategory);
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
	UCanvasPanelSlot* canvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(sender->GetParent());
	if (canvasSlot)
	{
		canvasSlot->GetPosition();
	}
	return;
	//canvasSlot->GetPosition();
//	itemDetails->SetPositionInViewport(sender->RenderTransform.Translation);
	//details menu
}

void UInventoryComponent::onItemButtonLeftHovered(UDataItemButton* sender)
{
	
	sender->RemoveChild(itemDetails);
	itemDetails->SetVisibility(ESlateVisibility::Hidden);
}

void UInventoryComponent::equip(UEquippedItemWidget* slot, UItemBase* base)
{
	UE_LOG(LogTemp, Warning, TEXT("EQUIP"));
	if (slot->allowedType != base->type && slot->allowedType != ItemCategory::ALL) return;
	bool t = true;
	if (slot->ItemButton->UserPointer) unEquip(slot, slot->ItemButton->UserPointer,NULL);
	equipDelegate.Execute(base, slot->slotEnum,t); //checking if char can equip
	if(!t) return;
	t = true;
	if (base->itemParent != this) itemMoved.ExecuteIfBound(base, base->itemParent, this, t); //check if listener like shopman allows it
	if (!t) return;
	slot->ItemButton->UserPointer = base;
	if (base->itemParent)
	{
		
		base->itemParent->removeItem(base);
		base->itemParent = this;

	}

	equipment->equipItem(slot->slotEnum, base);
	refresh();
}

void UInventoryComponent::moveItem(UItemWidget* slot, UItemBase* base, UItemScrollBoxWidget* sender)
{
	if (!base) return;
	bool t = true;
	if (base->itemParent != this) itemMoved.ExecuteIfBound(base, base->itemParent, this, t); //check if listener allows
	if (!t) return;
	base->itemParent->removeItem(base);
	sender->inventoryParent->addItem(base);
	
	refresh();
}
void UInventoryComponent::unEquip(UEquippedItemWidget* slot, UItemBase* base, UItemScrollBoxWidget* sender)
{
	bool t = true;
	unEquipDelegate.ExecuteIfBound(base, slot->slotEnum,t);
	if (!t) return;
	t = true;
	if (base->itemParent != sender->inventoryParent) itemMoved.ExecuteIfBound(base, base->itemParent, sender->inventoryParent, t); //check if listener allows
	if (!t) return;
	sender->inventoryParent->addItem(base);
	slot->ItemButton->UserPointer = NULL;
	equipment->unequipItem(slot->slotEnum);
	refresh();
}

void  UInventoryComponent::attachWeaponAttachment(UWeaponAttachmentWidget* slot, UItemBase* base)
{
	if (slot->allowedType != base->type && slot->allowedType != ItemCategory::ALL) return;

	UWeaponItem* x = equipment->getItem<UWeaponItem>(SlotInformation::WEAPON1);
	if (x)
	{
		if (x->generatedWeapon)
		{
			if (!x->generatedWeapon->attachAttachment(slot->weaponAttachmentSlot, Cast<UWeaponAttachmentItem>(base)))
			{
				slot->ItemButton->UserPointer = base;
				if (base->itemParent)
				{
					base->itemParent->removeItem(base);
					base->itemParent = this;
				}
			}
		}
	}
	
	refresh();
	
}


void  UInventoryComponent::detachWeaponAttachment(UWeaponAttachmentWidget* slot, UItemBase* base, UItemScrollBoxWidget* sender)
{
	UE_LOG(LogTemp, Warning, TEXT("UNEQUIP"));
	UWeaponItem* x = equipment->getItem<UWeaponItem>(SlotInformation::WEAPON1);
	if (x) if (x->generatedWeapon)
	{
		
		x->generatedWeapon->detachAttachment(slot->weaponAttachmentSlot);
		
	}
	if (sender)
	{
		sender->inventoryParent->addItem(base);
	}
	else
	{
		addItem(base);
	}
	slot->ItemButton->UserPointer = NULL;
	refresh();
}
void  UInventoryComponent::openTransferWindow(UInventoryComponent* otherInventory)
{
	if (!mainInventory) loadUI();
	mainInventory->otherBorder->SetVisibility(ESlateVisibility::Visible);
	mainInventory->otherItemBoxWidget->inventoryParent = otherInventory;
	otherInventoryForTransfering = otherInventory;
	showInventory();
	
	refresh();
	GetWorld()->GetTimerManager().SetTimer(transferRangeTimer, this, &UInventoryComponent::checkRangeOnOtherInventory, 0.1f, true);

}
void UInventoryComponent::closeTransferWindow()
{
	mainInventory->otherBorder->SetVisibility(ESlateVisibility::Hidden);
	mainInventory->otherItemBoxWidget->inventoryParent = this;
	otherInventoryForTransfering = NULL;
}
bool  UInventoryComponent::isItemInInvenotry(UItemBase* Item)
{
	if (items.Find(Item->type) == NULL) return true;
	if (items[Item->type].Find(Item) != NULL) return true;
	return false;
}
void UInventoryComponent::calculateWeight()
{
	currentWeight = 0;
	for (auto& x : items)
	{
		for (auto& y : x.Value)
		{
			currentWeight += y.Key->getWeight();
		}
	}
	currentWeight += equipment->getWeight();
}

float UInventoryComponent::getWeight()
{
	calculateWeight();
	return currentWeight;
}

















UCharacterEquipment::UCharacterEquipment()
{
	for (uint8 i = 0; i < (uint8)SlotInformation::NUM;i++)
	{ 
		equipment.Add(NULL);
	}
}
UCharacterEquipment::~UCharacterEquipment()
{

}
void UCharacterEquipment::equipItem(SlotInformation slot, UItemBase* item)
{
	equipment[(uint8)slot] = item;
}
UItemBase* UCharacterEquipment::unequipItem(SlotInformation slot)
{
	UItemBase* x = getItem<UItemBase>(slot);
	equipment[(uint8)slot] = NULL;
	return x;

}
template <typename T>
T* UCharacterEquipment::getItem(SlotInformation slot)
{

	if((uint8)SlotInformation::NUM > (uint8)slot && (uint8)slot >= 0) return Cast<T>(equipment[(uint8)slot]);
	return NULL;
}

float UCharacterEquipment::getWeight()
{
	float y = 0;
	for (int i = 0; i < equipment.Num();i++)
	{
		if (equipment[i]) y += equipment[i]->weight;
	}
	return y;
}

