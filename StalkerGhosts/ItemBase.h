// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Actor.h"
#include "ItemWidget.h"
#include "ItemEnums.h"

//#include "DataTables.h"
#include "ItemBase.generated.h"
class AStalkerGhostsCharacter;
struct FItemLookUpTable;
class UBuff;
UCLASS(Blueprintable, BlueprintType)
class UItemBase : public UObject
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

	virtual void use(AStalkerGhostsCharacter* user) {};
	virtual void equip(AStalkerGhostsCharacter* user) {};
	virtual void unEquip(AStalkerGhostsCharacter* user) {};
};

UCLASS(Blueprintable, BlueprintType)
class UArtifact : public UItemBase
{
	GENERATED_BODY()
public:
	UArtifact() { type = ItemCategory::ARTIFACT; }
	~UArtifact() {}

	TArray<UBuff*> attachedBuffs;
};
UCLASS(Blueprintable, BlueprintType)
class UNightVisionItem : public UItemBase
{
	GENERATED_BODY()
public:
	UNightVisionItem () { type = ItemCategory::NIGHTVISION; }
	~UNightVisionItem () {}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	UPostProcessComponent* nightvision;
};
class ABullet;
UCLASS(Blueprintable, BlueprintType)
class UBulletItem : public UItemBase
{
	GENERATED_BODY()
public:
	UBulletItem() { type = ItemCategory::AMMUNITION; }
	~UBulletItem() {}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	TSubclassOf<ABullet> bullet;
};
class AWeapon;
UCLASS(Blueprintable, BlueprintType)
class UWeaponItem : public UItemBase
{
	GENERATED_BODY()
public:
	UWeaponItem() { type = ItemCategory::AMMUNITION; }
	~UWeaponItem() {}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	TSubclassOf<AWeapon> wep;
};
class AGrenade;
UCLASS(Blueprintable, BlueprintType)
class UGrenadeItem : public UItemBase
{
	GENERATED_BODY()
public:
	UGrenadeItem() { type = ItemCategory::AMMUNITION; }
	~UGrenadeItem() {}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	TSubclassOf<AGrenade> gren;
};