// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ItemEnums.h"
#include "InventoryInterface.generated.h"
class UItemBase;

/**
 * 
 */
UINTERFACE(MinimalAPI)
class UInventoryInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IInventoryInterface
{
	GENERATED_IINTERFACE_BODY()

		//return if action was accepted
		virtual bool equipmentAdded(UItemBase* item, SlotInformation slot) { return true; };
		virtual bool equipmentRemoved(UItemBase* item, SlotInformation slot) { return true; };
};

