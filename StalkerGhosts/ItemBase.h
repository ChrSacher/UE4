// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Actor.h"
#include "ItemWidget.h"
#include "ItemEnums.h"
#include "DamageEnum.h"
#include "Buff.h"
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
		FString itemIdentifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		UItemWidget* widget;

	float getWeight();


	bool equals(UItemBase* other);
	void loadFromTable(FItemLookUpTable* table);

	UFUNCTION(BlueprintImplementableEvent)
		void useItem();
	UFUNCTION(BlueprintImplementableEvent)
		void equipItem();
	UFUNCTION(BlueprintImplementableEvent)
		void unEquipItem();

	virtual void use(AStalkerGhostsCharacter* user) { this->useItem(); };
	
	virtual void equip(AStalkerGhostsCharacter* user) { this->equipItem(); };
	
	virtual void unEquip(AStalkerGhostsCharacter* user) { this->unEquipItem(); };

};

UCLASS(Blueprintable, BlueprintType)
class UArtifact : public UItemBase
{
	GENERATED_BODY()
public:
	UArtifact() 
	{ 
		type = ItemCategory::ARTIFACT; 
	}
	~UArtifact() {}
protected:

	

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		bool init = false;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		TArray<UBuff*> attachedBuffs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		TArray<FBuffInit> buffInit;
	TArray<UBuff*>& getBuffs()
	{
		if (!init)
		{
			for (int32 i = 0; i < buffInit.Num(); i++)
			{
				UBuff* x = NewObject<UBuff>();
				x->load(buffInit[i]);;
				attachedBuffs.Add(x);
			}
			init = true;
		}
		return attachedBuffs;
	}
};

UCLASS(Blueprintable, BlueprintType)
class UArmorItem : public UItemBase
{
	GENERATED_BODY()
public:
	UArmorItem() 
	{
		type = ItemCategory::ARMOR;
		armorType = ArmorSubCategory::ARMOR;
		
	}

public:
	TArray<UBuff*>& getBuffs();
	~UArmorItem() {}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		TArray< FBuffInit> buffInit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		float ArmorValue = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		USkeletalMesh* mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		ArmorSubCategory armorType = ArmorSubCategory::ARMOR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		TArray<DamageBodyPart> affectedParts;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		TArray<UBuff*> attachedBuffs;	
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		bool init = false;
	
	
	
};

UCLASS(Blueprintable, BlueprintType)
class UBackPackItem : public UArmorItem
{
	GENERATED_BODY()
public:
	UBackPackItem()
	{ 
		armorType = ArmorSubCategory::BACKPACK;
	}
	~UBackPackItem() {}
	
};

UCLASS(Blueprintable, BlueprintType)
class UBodyArmorItem : public UArmorItem
{
	GENERATED_BODY()
public:
	UBodyArmorItem() 
	{
		armorType = ArmorSubCategory::BODYARMOR;
	}
	~UBodyArmorItem() {}
	

};

UCLASS(Blueprintable, BlueprintType)
class UHelmetItem : public UArmorItem
{
	GENERATED_BODY()
public:
	UHelmetItem() 
	{  
		armorType = ArmorSubCategory::HELMET;
	}
	~UHelmetItem() {}

	
};

UCLASS(Blueprintable, BlueprintType)
class UBootItem : public UArmorItem
{
	GENERATED_BODY()
public:
	UBootItem() 
	{ 
		armorType = ArmorSubCategory::BOOTS;
	}
	~UBootItem() {}

	
};

UCLASS(Blueprintable, BlueprintType)
class UPantsItem : public UArmorItem
{
	GENERATED_BODY()
public:
	UPantsItem() 
	{ 
		armorType = ArmorSubCategory::PANTS;
	}
	~UPantsItem() {}


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