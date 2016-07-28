#pragma once

#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Components/ActorComponent.h"
#include "DamageEnum.h"
#include "Bullet.h"
#include "Grenade.h"
#include "Weapon.h"
#include "ItemEnums.h"
#include "DataTables.generated.h"

USTRUCT(Blueprintable)
struct FItemLookUpTable : public FTableRowBase
{
	GENERATED_BODY()
public:
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
		FString itemIdentifier = " ";

	UPROPERTY(EditAnywhere, Category = Item)
		FString dataTabelIdentifier = " ";
};



USTRUCT(Blueprintable)
struct FCategoryLookUpTable : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = Bullet)
		UTexture2D* CategoryPicture;

	UPROPERTY(EditAnywhere, Category = Bullet)
		FString categoryText  = "";
	UPROPERTY(EditAnywhere, Category = Bullet)
		ItemCategory category;
};


USTRUCT(Blueprintable)
struct FItemTable : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		FString item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		TSubclassOf<UItemBase> base;
};