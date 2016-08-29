// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Actor.h"
#include "ItemWidget.h"
#include "ItemEnums.h"
#include "DamageEnum.h"

//#include "DataTables.h"
#include "ItemBase.generated.h"
class AStalkerGhostsCharacter;
struct FItemLookUpTable;
class UBuff;
class UInventoryComponent;
class UItemDetailBaseWidget;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		UInventoryComponent* itemParent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		TSubclassOf<UItemDetailBaseWidget>  detailsWidget;
	float getWeight();


	bool equals(UItemBase* other);
	void loadFromTable(FItemLookUpTable* table);

	UFUNCTION(BlueprintImplementableEvent)
		void useItem();
	UFUNCTION(BlueprintImplementableEvent)
		void equipItem();
	UFUNCTION(BlueprintImplementableEvent)
		void unEquipItem();

	virtual void initialize(UWorld* world) {};
	virtual void use(AStalkerGhostsCharacter* user) {useItem(); };
	
	virtual void equip(AStalkerGhostsCharacter* user) { equipItem(); };
	
	virtual void unEquip(AStalkerGhostsCharacter* user) { unEquipItem(); };

};
UCLASS(Blueprintable, BlueprintType)
class UBuffHolderItem : public UItemBase
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		TArray<TSubclassOf<UBuff>> buffInit;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
		TArray<UBuff*> attachedBuffs;
	void initialize(UWorld* world);
public:
	virtual TArray<UBuff*> getBuffs();
};

UCLASS(Blueprintable, BlueprintType)
class UArtifact : public UBuffHolderItem
{
	GENERATED_BODY()
public:
	UArtifact() 
	{ 
		type = ItemCategory::ARTIFACT; 
	}
	~UArtifact() {}
protected:

public:
	
};

UCLASS(Blueprintable, BlueprintType)
class UArmorItem : public UBuffHolderItem
{
	GENERATED_BODY()
public:
	UArmorItem() 
	{
		type = ItemCategory::ARMOR;
		armorType = ArmorSubCategory::ARMOR;
		
	}

public:
	
	~UArmorItem() {}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		float ArmorValue = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		USkeletalMesh* mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		ArmorSubCategory armorType = ArmorSubCategory::ARMOR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		TArray<DamageBodyPart> affectedParts;
	
	
	
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

	bool hasNightVision = true;
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
	UWeaponItem() { type = ItemCategory::WEAPON; }
	~UWeaponItem() {}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		TSubclassOf<AWeapon> wep;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		AWeapon* generatedWeapon;

	void initialize(UWorld* world) override;
	
};
UCLASS(Blueprintable, BlueprintType)
class UWeaponAttachmentItem : public UItemBase
{
	GENERATED_BODY()

public:
	UWeaponAttachmentItem() { type = ItemCategory::WEAPON; }
	~UWeaponAttachmentItem() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		UStaticMesh* attachmentMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		TArray<FString> allowedWeapons;

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

USTRUCT(Blueprintable, BlueprintType)
struct STALKERGHOSTS_API FInventoryItemInitializer
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		TSubclassOf<UItemBase> item;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		float ammount;


};