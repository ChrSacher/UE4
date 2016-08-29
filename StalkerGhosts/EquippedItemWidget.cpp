// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "EquippedItemWidget.h"
#include "ItemWidget.h"
#include "InventoryComponent.h"
#include "MainGameInstance.h"


void UEquippedItemWidget::OnItemDrop(UDragDropOperation* operation)
{

	UItemWidget* x = Cast<UItemWidget>(operation->Payload);
	if (!x) return;
	if (x->ItemButton->UserPointer)
	{
		inventoryParent->equip(this, x->ItemButton->UserPointer);
	}
	//
}
void UEquippedItemWidget::loadVisuals()
{
	forceConstruct();
	if (!ItemButton) return;
	if (!ItemButton->UserPointer)
	{
		image->SetBrushFromTexture(NULL);
		return;
	}
	image->SetBrushFromTexture(ItemButton->UserPointer->picture);
}


void UWeaponAttachmentWidget::OnItemDrop(UDragDropOperation* operation)
{

	UItemWidget* x = Cast<UItemWidget>(operation->Payload);
	if (GEngine->GetWorld() != nullptr && GEngine->GetWorld()->GetGameInstance() != nullptr)
	{
		UStalkerMainGameInstance* gameInstance = Cast<UStalkerMainGameInstance>(GEngine->GetWorld()->GetGameInstance());
		if (gameInstance) gameInstance->addWidgetToMain(x);
	}
	if (!x) return;
	if (x->ItemButton->UserPointer)
	{
		inventoryParent->attachWeaponAttachment(this, x->ItemButton->UserPointer);
	}
	//
}


void UWeaponAttachmentWidget::loadVisuals()
{
	forceConstruct();
	if (!ItemButton) return;
	if (!ItemButton->UserPointer)
	{
		attachmentImage->SetBrushFromTexture(NULL);
		return;
	}
	attachmentImage->SetBrushFromTexture(ItemButton->UserPointer->picture);
}

UEquippedItemWidget* UEquippedItemWidget::isDragDetected()
{
	APlayerController* player = GetOwningPlayer();
	if (!player) return NULL;
	UEquippedItemWidget* x = CreateWidget<UEquippedItemWidget>(GetWorld(), GetClass());

	x->AddToViewport();
	x->RemoveFromViewport();
	x->forceConstruct();
	x->ItemButton->UserPointer = ItemButton->UserPointer;
	x->loadVisuals();

	return x;
}

UWeaponAttachmentWidget*  UWeaponAttachmentWidget::isDragDetected()
{
	APlayerController* player = GetOwningPlayer();
	if (!player) return NULL;
	UWeaponAttachmentWidget* x = CreateWidget<UWeaponAttachmentWidget>(GetWorld(), GetClass());

	x->AddToViewport();
	x->RemoveFromViewport();
	x->forceConstruct();
	x->ItemButton->UserPointer = ItemButton->UserPointer;
	x->loadVisuals();

	return x;
}