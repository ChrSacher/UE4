// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/ActorComponent.h"
#include "ItemWidget.h"
#include "ItemEnums.h"
#include "DataTables.h"
#include "ItemBase.generated.h"


UCLASS()
class UItemBase : public UActorComponent
{
	GENERATED_BODY()
public:
	UItemBase();
	~UItemBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		UTexture2D* picture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		float weight = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		int32 ammount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		int32 maxAmmount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		FString name = "Default";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		UStaticMesh* groundMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		float worth = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		ItemCategory type = ItemCategory::ITEM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		FString dataTabelIdentifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		FString itemIdentifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		UItemWidget* widget;

	

	bool equals(UItemBase* other);
	void loadFromTable(FItemLookUpTable* table);
};

