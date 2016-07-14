// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/ActorComponent.h"
#include "MainInventoryWidget.h"
#include "ItemBase.generated.h"

UENUM(BlueprintType)
enum class ItemCategory : uint8
{
	ALL = 0,
	WEAPON = 1,
	ARMOR = 2,
	BOOTS = 3,
	GLOVES =4,
	ITEM = 5,
	MEDICAL = 6,
	FOOD = 7,
	GRENADES = 8,
	NUM = 9
};
UCLASS()
class UItemBase : public UActorComponent
{
	GENERATED_BODY()
public:
	UItemBase();
	~UItemBase();

	UPROPERTY(EditAnywhere, Category = Item)
		UTexture2D* picture;

	UPROPERTY(EditAnywhere, Category = Item)
		float weight = 1;

	UPROPERTY(EditAnywhere, Category = Item)
		uint16 ammount = 1;

	UPROPERTY(EditAnywhere, Category = Item)
		uint16 maxAmmount = 1;

	UPROPERTY(EditAnywhere, Category = Item)
		FString name = "Default";

	UPROPERTY(EditAnywhere, Category = Item)
		UStaticMesh* groundMesh;

	UPROPERTY(EditAnywhere, Category = Item)
		float worth = 50;

	UPROPERTY(EditAnywhere, Category = Item)
		ItemCategory type = ItemCategory::ITEM;

	UPROPERTY(EditAnywhere, Category = Item)
		FString dataTabelIdentifier;

	UPROPERTY(EditAnywhere, Category = Item)
		UItemWidget* widget;

	bool equals(UItemBase* other);

};

