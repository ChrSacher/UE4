// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/ActorComponent.h"
#include "ItemBase.generated.h"


UENUM(BlueprintType)
enum class ItemCategory : uint8
{
	WEAPON = 0,
	ARMOR = 1,
	BOOTS = 2,
	GLOVES = 3,
	ITEM = 4,
	MEDICAL = 5,
	FOOD = 6,
	NUM = 7
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
		float ammount = 1;

	UPROPERTY(EditAnywhere, Category = Item)
		float maxAmmount = 1;

	UPROPERTY(EditAnywhere, Category = Item)
		FString name = "Default";

	UPROPERTY(EditAnywhere, Category = Item)
		UStaticMesh* groundMesh;

	UPROPERTY(EditAnywhere, Category = Item)
		float worth = 50;

	UPROPERTY(EditAnywhere, Category = Item)
		int8 xSize = 5;

	UPROPERTY(EditAnywhere, Category = Item)
		int8 ySize = 5;
	UPROPERTY(EditAnywhere, Category = Item)
		ItemCategory type = ItemCategory::ITEM;

	bool equals(UItemBase* other);
};

