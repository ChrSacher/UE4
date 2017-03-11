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
#include "MainGameInstance.h"
#include "InventoryInterface.h"
#include "ItemDetailWidget.h"

UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = false;
	for (uint8 i = 0; i < uint8(ItemCategory::NUM);i++)
		items.Add(static_cast<ItemCategory>(i),FItemMap());

	equipment = CreateDefaultSubobject<UCharacterEquipment>(TEXT("equipment"));
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	equipment = NewObject<UCharacterEquipment>(this, equipment->GetClass());
	for (int i = 0; i < beginPlayEquipment.Num(); i++)
	{

		if (beginPlayEquipment[i].item)
		{
			UItemBase* x = NewObject<UItemBase>(this, beginPlayEquipment[i].item);
			if (x)
			{
				x->ammount = beginPlayEquipment[i].ammount;
				x->initialize(GetOwner()->GetWorld());
				addItem(x);
			}
		}
	}
	
	equipment->attachedInventory = this;
	if (requiresUI) loadUI();
	
	showInventory();
	hideInventory();
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
	if (!Item) return false;
	items[Item->type].map.Add(Item, Item);
	currentWeight += Item->weight * Item->ammount;
	
	if (requiresUI)
	{
		if (Item->widget) Item->widget->RemoveFromParent();
		UItemWidget* x = CreateWidget<UItemWidget>(GetWorld(), itemTemplate);
		if (x)
		{
			x->AddToViewport();
			x->itemBase = Item;
			x->loadVisuals();
			x->RemoveFromViewport();
		}

		Item->widget = x;
	}

	
	 //THIS IS A WORKAROUND FOR UE4 BUG WITH WIDGEWT CONSTRUCTION
	//mainInventory->ItemBox->AddChild(x)->SetSize(FSlateChildSize());

	return true;
}
bool UInventoryComponent::addItemByName(FString name, int32 ammount, bool forceNew )
{
	UItemBase* base = createItem<UItemBase>(name);
	base->ammount = ammount;
	return addItem(base);

}

bool UInventoryComponent::addItemByClass(TSubclassOf<UItemBase> item, int32 ammount, bool forceNew )
{
	UItemBase* x = NewObject<UItemBase>(this, item);
	x->initialize(GetOwner()->GetWorld());
	return addItem(x);
}

bool UInventoryComponent::addItem(UItemBase* Item,bool forceNew)
{
	if (!Item) return true;
	
	if (items.Find(Item->type) == NULL) return false;
	if (items[Item->type].map.Find(Item) != NULL)
	{
		return true;
	}
	if (forceNew)
	{
		addItemCreate(Item);
	}
	else
	{
		
		
			for (auto& x : items[Item->type].map)
			{
				if (x.Key->name == Item->name)
				{
					int32 ammountPossibleToAdd = x.Key->maxAmmount - x.Key->ammount;
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
					if (requiresUI)
						if(x.Key->widget) x.Key->widget->loadVisuals();
				}
				
			}
			if (Item->ammount > 0)
			{
				addItemCreate(Item);
			}
		
	}
	if (requiresUI)
		if (Item->widget) Item->widget->loadVisuals();
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
	T* x = Cast<T>(NewObject<UItemBase>(this, row->base));
	Cast<UItemBase>(x)->initialize(GetOwner()->GetWorld());
	return x;
}
bool UInventoryComponent::removeItem(UItemBase* Item, int32 ammount)
{
	if (!Item) return false;
	if (ammount > 0)
	{
		UItemBase* place = items[Item->type].map[Item];
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
	items[Item->type].map.Remove(Item);
	equipment->unequipItemByPointer(Item);//used only when character needs to be equipped while looting
	refresh();
	calculateWeight();
	return true;

}
int32 UInventoryComponent::getItemCount(FString& itemName)
{
	TArray<UItemBase*> item = lookForItems(itemName);
	int32 maxAmm = 0;
	for (int32 i = 0; i < item.Num(); i++)
	{
		maxAmm += item[i]->ammount;
	}
	return maxAmm;
}
TArray<UItemBase*>  UInventoryComponent::removeItems(FString& itemName, int32 ammount)
{
	TArray<UItemBase*> item = lookForItems(itemName);
	if (ammount < 0)
	{
		for (int32 i = 0; i < item.Num(); i++)
		{
			removeItem(item[i]);
		}
		return item;
	}
	int32 maxAmm = getItemCount(itemName);
	if (ammount > maxAmm)
	{
		TArray<UItemBase*> returnItems;
		for (int32 i = 0; i < item.Num(); i++)
		{
			if (maxAmm > item[i]->ammount)
			{
				maxAmm -= item[i]->ammount;
				returnItems.Add(item[i]);
			}
			else
			{
				item[i]->ammount -= maxAmm;
				break;
			}
		}
		return returnItems;
	}
	else
	{
		return TArray<UItemBase*>();
	}
}

TArray<UItemBase*>   UInventoryComponent::getItemsByAmmount(FString itemName, int32 ammount)
{
	if (ammount < 0) return TArray<UItemBase*>();
	TArray<UItemBase*> item = lookForItems(itemName);
	

	int32 maxAmm = ammount;
	TArray<UItemBase*> returnItems;
	for (int32 i = 0; i < item.Num(); i++)
	{
		
		if (maxAmm >= item[i]->ammount)
		{
			maxAmm -= item[i]->ammount;
			returnItems.Add(item[i]);
			removeItem(item[i]);
			
		}
		else
		{
			UItemBase* x = NewObject<UItemBase>(this, item[i]->GetClass());
			x->ammount = maxAmm;
			item[i]->ammount -= maxAmm;
			returnItems.Add(x);
			
			maxAmm = 0;
		}
		if (maxAmm <= 0) break;
	}
	return returnItems;
}
void UInventoryComponent::combineItems(TArray<UItemBase*> &item)
{
	
	for (int32 i = 0; i < item.Num(); i++)
	{
		int32 maxAmm = item[i]->maxAmmount - item[i]->ammount;
		for (int32 o = i + 1; o < item.Num(); o++)
		{
			if (item[o]->itemIdentifier == item[i]->itemIdentifier && item[o]->ammount > 0)
			{
				if (maxAmm >= item[o]->ammount)
				{
					maxAmm -= item[o]->ammount;
					item[o]->ammount = 0;

				}
				else
				{
					item[o]->ammount -= maxAmm;
					maxAmm = 0;
					

				}
			}
		}
	}
}
void UInventoryComponent::dropItem(UItemBase* Item)
{
	if (!Item) return;
	items[Item->type].map.Remove(Item);
	if(Item->ammount > 0) GetWorld()->SpawnActor<AItemBaseActor>(itemBaseTemplate, GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation())->spawn(Item);
	if (requiresUI)
		Item->widget->RemoveFromParent();
	calculateWeight();
}

UItemBase* UInventoryComponent::splitItem(UItemBase* Item,float ratio)
{
	if (!Item) return NULL;
	if(Item->ammount < 2) return Item;
	UItemBase* newItem = NewObject<UItemBase>(this, Item->GetClass());
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
		for (auto& y : x.Value.map)
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
		for (auto& y : x.Value.map)
		{
			if (y.Key->itemIdentifier == name) vec.Add(y.Value);
		}
	}
	return vec;
}
void UInventoryComponent::loadCategories()
{
	if (!mainInventory || !requiresUI ) return;
	
		static const FString ContextString(TEXT("GENERAL"));
		for (uint8 i = 0; i < mainInventory->categories.Num(); i++)
		{
			if (!mainInventory->categories[i]) continue;
			mainInventory->categories[i]->cat = ItemCategory(i);
			mainInventory->categories[i]->OnClicked.AddDynamic(this, &UInventoryComponent::onCategoryClicked);

		}
		for (uint8 i = 0; i < mainInventory->otherCategories.Num(); i++)
		{
			if (!mainInventory->otherCategories[i]) continue;
			mainInventory->otherCategories[i]->cat = ItemCategory(i);
			mainInventory->otherCategories[i]->OnClicked.AddDynamic(this, &UInventoryComponent::onOtherCategoryClicked);

		}
	
}
bool UInventoryComponent::isVisible()
{
	if (!requiresUI) return false;
	return mainInventory->GetVisibility() == ESlateVisibility::Visible;
}
void UInventoryComponent::loadUI()
{
	if (!requiresUI) return;
	if (!mainInventory)
	{
		if (mainInventoryTemplate)
		{
			mainInventory = CreateWidget<UMainInventoryWidget>(GetWorld(), mainInventoryTemplate);
			if (!mainInventory)
			{
				return;
				requiresUI = false;
			}		
			
			mainInventory->AddToViewport();
		}
		else
		{
			requiresUI = false;
			return;
		}
	}
	
	//UStalkerMainGameInstance* gameInstance = Cast<UStalkerMainGameInstance>(GetOwner()->GetWorld()->GetGameInstance());
	//if(gameInstance) gameInstance->addWidgetToMain(mainInventory);
	loadCategories();	
	mainInventory->SetVisibility(ESlateVisibility::Hidden);
	mainInventory->otherItemBoxWidget->inventoryParent = this;
	mainInventory->sellItemBoxWidget->inventoryParent = this;
	mainInventory->ItemBoxWidget->inventoryParent = this;
	mainInventory->buyItemBoxWidget->inventoryParent = this;
	equipment->equipmentWidget = mainInventory->equipmentWidgetTransfer;
	equipment->setupUI();
	refresh();
	for (uint8 i = 0; i < (uint8)SlotInformation::NUM; i++)
	{
		equipment->equipmentWidget->widgets[i]->inventoryParent = this;
	}
	
	mainInventory->tradeButton->OnClicked.RemoveDynamic(this, &UInventoryComponent::tradeItems);
	mainInventory->resetButton->OnClicked.RemoveDynamic(this, &UInventoryComponent::resetItems);
	mainInventory->closeButton->OnClicked.RemoveDynamic(this, &UInventoryComponent::closeTradeWindow);

	mainInventory->tradeButton->OnClicked.AddDynamic(this, &UInventoryComponent::tradeItems);
	mainInventory->resetButton->OnClicked.AddDynamic(this, &UInventoryComponent::resetItems);
	mainInventory->closeButton->OnClicked.AddDynamic(this, &UInventoryComponent::closeTradeWindow);

	mainInventory->equipmentWidgetTransfer->weaponWidget->attachment1->inventoryParent = this;
	mainInventory->equipmentWidgetTransfer->weaponWidget->attachment2->inventoryParent = this;
	mainInventory->equipmentWidgetTransfer->weaponWidget->attachment3->inventoryParent = this;
	mainInventory->equipmentWidgetTransfer->weaponWidget->weapon->inventoryParent = this;
}
void UInventoryComponent::loadItemWidget(UItemBase* item, UItemScrollBoxWidget* scrollBox)
{


	if (!requiresUI) return;
	if (!item) return;
	if (!item->widget)
	{
		item->widget = CreateWidget<UItemWidget>(GetWorld(), itemTemplate);
		if (!item->widget) return;
	}
	
	scrollBox->addAsChild(item);
	item->widget->loadVisuals();
	item->widget->itemBase = item;
	item->widget->OnClicked.Clear();
	item->widget->OnClicked.AddDynamic(this, &UInventoryComponent::onItemButtonClicked);
	item->widget->OnHovered.Clear();
	item->widget->OnHovered.AddDynamic(this, &UInventoryComponent::onItemButtonHovered);
	item->widget->OnUnHovered.Clear();
	item->widget->OnUnHovered.AddDynamic(this, &UInventoryComponent::onItemButtonLeftHovered);

}
void UInventoryComponent::refresh()
{
	if (!requiresUI) return;
	if(mainInventory)	mainInventory->ItemBoxWidget->itemBox->ClearChildren();
	for (auto& s : items)
	{
		if (s.Key == currentCategory || currentCategory == ItemCategory::ALL)
		{
			for (auto& w : s.Value.map)
			{

				loadItemWidget(w.Key, mainInventory->ItemBoxWidget);

			}
		}
		
	}
	
	if (otherInventoryForTransfering)
	{
		if (mainInventory)	mainInventory->otherItemBoxWidget->itemBox->ClearChildren();
		for (auto& s : otherInventoryForTransfering->items)
		{
			if (s.Key == otherCurrentCategory || otherCurrentCategory == ItemCategory::ALL)
			{
				for (auto& w : s.Value.map)
				{

					loadItemWidget(w.Key, mainInventory->otherItemBoxWidget);

				}
			}

		}
	}
	for (int32 i = 0; i < itemsToSell.Num(); i++)
	{
		loadItemWidget(itemsToSell[i], mainInventory->sellItemBoxWidget);
	}
	for (int32 i = 0; i < itemsToBuy.Num(); i++)
	{
		loadItemWidget(itemsToBuy[i], mainInventory->buyItemBoxWidget);
	}
}


void UInventoryComponent::setVisiblity(bool Vis)
{
	if (!requiresUI) return;
	if(Vis) mainInventory->RemoveFromViewport();
	if (!Vis)
	{
		mainInventory->AddToViewport();
		closeTransferWindow();
	}
}

void UInventoryComponent::showInventory()
{
	if (!requiresUI) return;
	
	if (mainInventory) mainInventory->SetVisibility(ESlateVisibility::Visible);
	mainInventory->otherBorder->SetVisibility(ESlateVisibility::Hidden);
	mainInventory->inventoryBorder->SetVisibility(ESlateVisibility::Visible);
	mainInventory->equipmentBorder->SetVisibility(ESlateVisibility::Visible);
	mainInventory->tradeBorder->SetVisibility(ESlateVisibility::Collapsed);
	refresh();
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
	if (!requiresUI) return;
	if (mainInventory) mainInventory->SetVisibility(ESlateVisibility::Hidden);
	closeTransferWindow();
	closeTradeWindow();
	GetWorld()->GetTimerManager().ClearTimer(transferRangeTimer);
}
void UInventoryComponent::onCategoryClicked(UItemCategoryWidget* sender)
{
	
	mainInventory->otherItemBoxWidget->itemBox->ClearChildren();
	currentCategory = sender->cat;
	auto it = items.Find(currentCategory);
	if (it)
	{
		refresh();
	}
	
}
void UInventoryComponent::onOtherCategoryClicked(UItemCategoryWidget* sender)
{
	mainInventory->otherItemBoxWidget->itemBox->ClearChildren();
	otherCurrentCategory = sender->cat;
	auto it = items.Find(otherCurrentCategory);
	if (it)
	{
		refresh();
	}
}
void UInventoryComponent::onItemButtonClicked(UItemWidget* sender)
{
	selectedItem = sender->itemBase;
}

void UInventoryComponent::onItemButtonHovered(UItemWidget* sender)
{

	if (!sender) return;
	if (!sender->itemDetails) if(sender->itemBase->detailsWidget) sender->itemDetails = CreateWidget<UItemDetailBaseWidget>(GetWorld(), sender->itemBase->detailsWidget);
	if (sender->itemDetails)
	{

		sender->itemDetails->load(sender->itemBase);
		sender->ItemButton->SetToolTip(sender->itemDetails);
	}

}
void UInventoryComponent::onItemButtonHoveredTimer()
{
	
}
void UInventoryComponent::onItemButtonLeftHovered(UItemWidget* sender)
{

}

void UInventoryComponent::equip(UEquippedItemWidget* slot, UItemBase* base)
{
	UE_LOG(LogTemp, Warning, TEXT("EQUIP"));
	if (slot->allowedType != base->type && slot->allowedType != ItemCategory::ALL) return;
	if (equipment->isItemEquippable(slot->equipmentSlot, base))
	{
		if(state ==  InventoryState::TRADE)
			{
			if (base->itemParent == mainInventory->otherItemBoxWidget || base->itemParent == mainInventory->buyItemBoxWidget)
			{
					UE_LOG(LogTemp, Warning, TEXT("Item has to be bought first!"));
					return;
			}
		}
		if (base->itemParent == mainInventory->sellItemBoxWidget)
		{
			unsellItem(base);
		}
		if (base->itemParent == mainInventory->ItemBoxWidget)
		{
			
			removeItem(base);

		}
		if (base->itemParent == mainInventory->otherItemBoxWidget)
		{

			otherInventoryForTransfering->removeItem(base);

		}
	
		
		equipment->equipItem(slot->equipmentSlot, base);
		
	}
	refresh();
}

void UInventoryComponent::moveItem(UItemWidget* slot, UItemBase* base, UItemScrollBoxWidget* sender)
{

	
	if (sender == mainInventory->sellItemBoxWidget)
	{
		if (state == InventoryState::TRADE)
		{
			if (base->itemParent != mainInventory->ItemBoxWidget) return;
			removeItem(base);
			sellItem(base);
		}
	}
	if (sender == mainInventory->ItemBoxWidget)
	{
		if (state == InventoryState::TRADE)
		{
			if (base->itemParent != mainInventory->sellItemBoxWidget) return;
			addItem(base);
			unsellItem(base);
		}
		if (state == InventoryState::TRANSFER)
		{
			if (!(base->itemParent == mainInventory->otherItemBoxWidget)) return;
			
			otherInventoryForTransfering->removeItem(base);
			addItem(base);
			
		
		}
	}
	if (sender == mainInventory->otherItemBoxWidget)
	{

		if (state == InventoryState::TRADE)
		{
			if (base->itemParent != mainInventory->buyItemBoxWidget) return;
			otherInventoryForTransfering->addItem(base);
			unBuyItem(base);
		}
		if (state == InventoryState::TRANSFER)
		{

			base->itemParent->inventoryParent->removeItem(base);
			otherInventoryForTransfering->addItem(base);
		}

	}
	if (sender == mainInventory->buyItemBoxWidget)
	{
		if (state == InventoryState::TRADE)
		{
			if (base->itemParent != mainInventory->otherItemBoxWidget) return;
			otherInventoryForTransfering->removeItem(base);
			buyItem(base);
		}

	}
	refresh();
	
}
void UInventoryComponent::unEquip(UEquippedItemWidget* slot, UItemBase* base, UItemScrollBoxWidget* sender)
{
	UE_LOG(LogTemp, Warning, TEXT("unEQUIP"));
	if (slot->allowedType != base->type && slot->allowedType != ItemCategory::ALL) return;
	if (sender == mainInventory->sellItemBoxWidget)
	{
		if (state == InventoryState::TRADE)
		{
			if (base->itemParent != mainInventory->ItemBoxWidget) return;
			sellItem(base);
		}
	}
	if (sender == mainInventory->ItemBoxWidget)
	{

		addItem(base);
	
	}
	if (sender == mainInventory->otherItemBoxWidget)
	{
		if (state == InventoryState::TRANSFER)
		{

			
			otherInventoryForTransfering->addItem(base);
		}

	}
	if (sender == mainInventory->buyItemBoxWidget)
	{

	}
	equipment->unequipItem(slot->equipmentSlot);
	refresh();
}

void  UInventoryComponent::attachWeaponAttachment(UWeaponAttachmentWidget* slot, UItemBase* base)
{
	
	if (base->itemParent == mainInventory->otherItemBoxWidget)
	{
		if (state == InventoryState::TRADE) return;

	}
	if (base->itemParent == mainInventory->buyItemBoxWidget)
	{
		return;
	}
	UWeaponItem* x = equipment->getItem<UWeaponItem>(EquipmentInformation::WEAPON1);
	if (x) 
		if (x->generatedWeapon->attachAttachment(slot->weaponAttachmentSlot, Cast<UWeaponAttachmentItem>(base)))
		{
			if (base->itemParent == mainInventory->sellItemBoxWidget)
			{
				itemsToSell.Remove(base);
			}
			if (base->itemParent == mainInventory->ItemBoxWidget)
			{
				removeItem(base);
			}
			if (base->itemParent == mainInventory->otherItemBoxWidget)
			{
				otherInventoryForTransfering->removeItem(base);
			}
			slot->itemBase = base;
			slot->loadVisuals();
			
		}


	refresh();
}


void  UInventoryComponent::detachWeaponAttachment(UWeaponAttachmentWidget* slot, UItemBase* base, UItemScrollBoxWidget* sender)
{
	if (!base) return;
	if (sender == mainInventory->sellItemBoxWidget)
	{
		sellItem(base);
		slot->itemBase = base;
		slot->loadVisuals();

	}
	if (sender == mainInventory->ItemBoxWidget)
	{
		addItem(base);
	}
	if (sender == mainInventory->otherItemBoxWidget)
	{

		if (state == InventoryState::TRANSFER)
		{
			otherInventoryForTransfering->addItem(base);
		}

	}
	UWeaponItem* x = equipment->getItem<UWeaponItem>(EquipmentInformation::WEAPON1);
	if (x)
		x->generatedWeapon->detachAttachment(slot->weaponAttachmentSlot);
}




void  UInventoryComponent::openTransferWindow(UInventoryComponent* otherInventory)
{
	
	if (!requiresUI) return;
	state = InventoryState::TRANSFER;
	showInventory();
	
	mainInventory->otherBorder->SetVisibility(ESlateVisibility::Visible);
	mainInventory->inventoryBorder->SetVisibility(ESlateVisibility::Visible);
	mainInventory->equipmentBorder->SetVisibility(ESlateVisibility::Visible);
	mainInventory->tradeBorder->SetVisibility(ESlateVisibility::Collapsed);
	otherInventoryForTransfering = otherInventory;
	
	
	refresh();
	GetWorld()->GetTimerManager().SetTimer(transferRangeTimer, this, &UInventoryComponent::checkRangeOnOtherInventory, 0.1f, true);

}

void UInventoryComponent::closeTransferWindow()
{
	state = InventoryState::INVENTORY;
	if (mainInventory)
	{
		mainInventory->otherBorder->SetVisibility(ESlateVisibility::Hidden);
		mainInventory->tradeBorder->SetVisibility(ESlateVisibility::Collapsed);
	}
	otherInventoryForTransfering = NULL;
}

void UInventoryComponent::closeTradeWindow()
{
	state = InventoryState::INVENTORY;
	if (mainInventory)
	{
		mainInventory->tradeBorder->SetVisibility(ESlateVisibility::Collapsed);
		mainInventory->otherBorder->SetVisibility(ESlateVisibility::Hidden);
	}
	resetItems();
	otherInventoryForTransfering = NULL;
}
void UInventoryComponent::openTradeWindow(UInventoryComponent* otherInventory)
{
	if (!requiresUI) return;
	state = InventoryState::TRADE;
	showInventory();
	mainInventory->otherBorder->SetVisibility(ESlateVisibility::Visible);
	mainInventory->inventoryBorder->SetVisibility(ESlateVisibility::Visible);
	mainInventory->equipmentBorder->SetVisibility(ESlateVisibility::Collapsed);
	mainInventory->tradeBorder->SetVisibility(ESlateVisibility::Visible);
	otherInventoryForTransfering = otherInventory;

	refresh();
	GetWorld()->GetTimerManager().SetTimer(transferRangeTimer, this, &UInventoryComponent::checkRangeOnOtherInventory, 0.1f, true);
}
void UInventoryComponent::tradeItems()
{
	for (int32 i = 0; i < itemsToSell.Num(); i++)
	{
		if (otherInventoryForTransfering)
		{
			otherInventoryForTransfering->addItem(itemsToSell[i]);
		}
	}
	itemsToSell.Empty();
	for (int32 i = 0; i < itemsToBuy.Num(); i++)
	{
		addItem(itemsToBuy[i]);
	}
	itemsToBuy.Empty();
	refresh();
}
void UInventoryComponent::resetItems()
{
	for (int32 i = 0; i < itemsToSell.Num(); i++)
	{
		addItem(itemsToSell[i]);
	}
	itemsToSell.Empty();
	for (int32 i = 0; i < itemsToBuy.Num(); i++)
	{
		if (otherInventoryForTransfering)
		{
			otherInventoryForTransfering->addItem(itemsToBuy[i]);
		}
	}
	itemsToBuy.Empty();
	refresh();
}

void UInventoryComponent::sellItem(UItemBase* Item)
{
	itemsToSell.Add(Item);
}
void UInventoryComponent::buyItem(UItemBase* Item)
{
	itemsToBuy.Add(Item);
}
void  UInventoryComponent::unsellItem(UItemBase* item)
{
	itemsToSell.Remove(item);
}
void  UInventoryComponent::unBuyItem(UItemBase* Item)
{
	itemsToBuy.Remove(Item);
}
bool  UInventoryComponent::isItemInInvenotry(UItemBase* Item)
{
	if (items.Find(Item->type) == NULL) return true;
	if (items[Item->type].map.Find(Item) != NULL) return true;
	return false;
}
void UInventoryComponent::calculateWeight()
{
	int32 weight = currentWeight;
	currentWeight = 0;
	for (auto& x : items)
	{
		for (auto& y : x.Value.map)
		{
			currentWeight += y.Key->getWeight();
		}
	}
	currentWeight += equipment->getWeight();
	if(weight != currentWeight) OnWeightChanged.ExecuteIfBound(currentWeight);
}

float UInventoryComponent::getWeight()
{
	calculateWeight();
	return currentWeight;
}
















void UCharacterEquipment::setupUI()
{
	for (int i = 0; i < (uint8)EquipmentInformation::NUM; i++)
	{
		
		//equipmentWidget->widgets.Add(NULL);
	}

}
UCharacterEquipment::UCharacterEquipment()
{
	for (uint8 i = 0; i < (uint8)EquipmentInformation::NUM;i++)
	{ 
		equipment.Add(NULL);
		FEquipmentInit x;
		x.slot = (EquipmentInformation)i;
		initEquipment.Add(x);
	}

}
UCharacterEquipment::~UCharacterEquipment()
{
}
void UCharacterEquipment::initializeStartEquipment()
{
	for (int32 i = 0; i < initEquipment.Num(); i++)
	{
		if (initEquipment[i].item)
		{
			if (isItemEquippable(initEquipment[i].slot,Cast<UItemBase>( initEquipment[i].item->GetDefaultObject())))
			{
				equipItem(initEquipment[i].slot, NewObject<UItemBase>(this,initEquipment[i].item));
			}
		}
	}
}
void UCharacterEquipment::equipItem(EquipmentInformation slot, UItemBase* item)
{
	uint8 slotNumber = (uint8)slot;
	if (equipmentWidget)
	{
	
		
		if (equipment[slotNumber]) unequipItem(slot);
		if (slot == EquipmentInformation::WEAPON1)
		{
			UWeaponItem* w = Cast<UWeaponItem>(item);
			UWeaponAttachmentItem* z = NULL;
			z = w->generatedWeapon->getAttachment(WeaponAttachmentSlot::GRIP);
			if (z)
			{
				equipmentWidget->weaponWidget->attachment3->itemBase = z;
				equipmentWidget->weaponWidget->attachment3->loadVisuals();
			}
			z = w->generatedWeapon->getAttachment(WeaponAttachmentSlot::MUZZLEBRAKE);
			if (z)
			{
				equipmentWidget->weaponWidget->attachment2->itemBase = z;
				equipmentWidget->weaponWidget->attachment2->loadVisuals();
			}
			z = w->generatedWeapon->getAttachment(WeaponAttachmentSlot::SCOPE);
			if (z)
			{
				equipmentWidget->weaponWidget->attachment1->itemBase = z;
				equipmentWidget->weaponWidget->attachment1->loadVisuals();
			}
		}
		UEquippedItemWidget* s = equipmentWidget->getEquippedWidget(slot);
		s->itemBase = item;
		s->loadVisuals();
	}
	equipment[slotNumber] = item;
	equipDelegate.ExecuteIfBound(item,slot);
}
UItemBase* UCharacterEquipment::unequipItem(EquipmentInformation slot)
{
	UItemBase* x = getItem<UItemBase>(slot);
	equipment[(uint8)slot] = NULL;
	if (equipmentWidget)
	{
		
		
		UEquippedItemWidget* s = equipmentWidget->widgets[(uint8)slot];
		if (s)
		{
			s->itemBase = NULL;
			s->loadVisuals();
		}
	}
	unEquipDelegate.ExecuteIfBound(x,slot);
	return x;

}












UItemBase* UCharacterEquipment::unequipItemByPointer(UItemBase* item)
{
	for (int i = 0; i < equipment.Num(); i++)
	{
		if (item == equipment[i])
		{
			return unequipItem(EquipmentInformation(i));
		}
	} return NULL;
}
TArray<UItemBase*> UCharacterEquipment::dumbItems()
{
	return equipment;
}
template <typename T>
T* UCharacterEquipment::getItem(EquipmentInformation slot)
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

bool UCharacterEquipment::isItemEquippable(EquipmentInformation slot, UItemBase* Item)
{
	UEquippableItem* x = Cast<UEquippableItem>(Item);
	if(x)
	{
		bool y = UEquipmentSlotCompare::Equals(slot,x->slot ) && x->slot != SlotInformation::NON;
		return y;
	}
	return false;
}




void UCraftingSystemComponent::addRecipe(URecipe* recipe)
{
	if (!recipe) return;
	recipes.Add(recipe);
	loadRecipe(recipe);
	bindRecipe(recipe);
}
void UCraftingSystemComponent::BeginPlay()
{
	TArray<FCraftingTableRow*> rows;
	static const FString ContextString(TEXT("GENERAL"));
	if (!recipeList) return;
	recipeList->GetAllRows<FCraftingTableRow>(ContextString,rows);
	for (int32 i = 0; i < recipes.Num(); i++)
	{
		URecipe* newRecipe = NewObject<URecipe>(NULL, URecipe::StaticClass());
		newRecipe->displayName = rows[i]->displayName;
		newRecipe->identifierName = rows[i]->identifierName;
		newRecipe->requiredItems = rows[i]->requiredItems;
		newRecipe->finalProduct = rows[i]->finalProduct;
		newRecipe->ammount = rows[i]->ammount;

		addRecipe(newRecipe);
	}

}
void UCraftingSystemComponent::loadRecipe(URecipe* recipe)
{
	if (recipeTemplate) if (!recipe->associatedRecipeWidget)
	{
		recipe->associatedRecipeWidget = CreateWidget<URecipeWidget>(GetWorld(), recipeTemplate);
		
	}
}
void UCraftingSystemComponent::bindRecipe(URecipe* recipe)
{
	if (recipe->associatedRecipeWidget)
	{
		recipe->associatedRecipeWidget->onClicked.AddDynamic(this, &UCraftingSystemComponent::onRecipeClicked);
		//	recipe.associatedRecipeWidget->onHovered;
		//	recipe.associatedRecipeWidget->onUnHovered;
	}
}
void UCraftingSystemComponent::refresh()
{
	craftingMenuWidget->recipeScrollBox->ClearChildren();
	for (int32 i = 0; i < recipes.Num(); i++)
	{
		craftingMenuWidget->recipeScrollBox->AddChild(recipes[i]->associatedRecipeWidget);
	}

}

bool UCraftingSystemComponent::hasEnoughItems()
{
	if (!attachedInventory) return false;
	for (int32 i = 0; i < selectedRecipe->requiredItems.Num(); i++)
	{
		if( attachedInventory->getItemCount(selectedRecipe->requiredItems[i].itemName) < selectedRecipe->requiredItems[i].ammount) return false;
	}
	return true;
}

void UCraftingSystemComponent::craftItem(URecipe* recipe)
{
	if (hasEnoughItems())
	{
		for (int32 i = 0; i <	recipe->requiredItems.Num(); i++)
		{
			attachedInventory->getItemsByAmmount(recipe->requiredItems[i].itemName, recipe->requiredItems[i].ammount);
		}
		attachedInventory->addItemByClass(recipe->finalProduct, selectedRecipe->ammount);
	}
	
}

void UCraftingSystemComponent::onRecipeClicked(URecipeWidget* recipeWidget)
{
	selectedRecipe = recipeWidget->associatedRecipe;
	refresh();
}

void UCraftingSystemComponent::onCloseButtonPressed()
{
	closeCraftingMenu();
}

void UCraftingSystemComponent::onCraftButtonClicked()
{
	craftItem(selectedRecipe);
}

void UCraftingSystemComponent::closeCraftingMenu()
{
	craftingMenuWidget->SetVisibility(ESlateVisibility::Hidden);
	attachedInventory = NULL;
}

void UCraftingSystemComponent::openCraftingMenu(UInventoryComponent* inventory)
{
	if (!inventory) return;
	craftingMenuWidget->SetVisibility(ESlateVisibility::Visible);
	attachedInventory = inventory;
	refresh();

}
