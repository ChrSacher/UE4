// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ItemEnums.h"
#include "InventoryInterface.generated.h"
class UItemBase;

/**
 * 
 */

class AStalkerGhostsCharacter;
UENUM(BlueprintType)
enum class InventoryAcceptance : uint8
{
	ACCEPTED,
	DENIED,
	PARTIAL
};

UINTERFACE(MinimalAPI)
class UInventoryInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IInventoryInterface
{
	GENERATED_IINTERFACE_BODY()
public:
		//return if action was accepted
		virtual InventoryAcceptance equipmentAdded(UItemBase* item, SlotInformation slot) { return InventoryAcceptance::DENIED; };
		virtual InventoryAcceptance equipmentRemoved(UItemBase* item, SlotInformation slot) { return InventoryAcceptance::DENIED; };
};

