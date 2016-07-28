#pragma once
#include "ItemBase.h"
#include "Components/ActorComponent.h"
#include "MainInventoryWidget.h"
#include "ItemCategoryWidget.h"
#include "DataTables.h"
#include "EquippedItemWidget.h"
#include "InventoryComponent.generated.h"

class AWeapon;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STALKERGHOSTS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	
	// Called when the game starts
	virtual void BeginPlay() override;
	// Called every frameini
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;


	UPROPERTY(EditAnywhere, Category = Inventory)
		uint16 maxWeight = 60;

	UPROPERTY(EditAnywhere, Category = Inventory)
		uint16 currentWeight;
	
	
	TMap<ItemCategory, TMap<UItemBase*,UItemBase*>> items;
	TMap<ItemCategory, TMap<UItemBase*, UItemBase*>> equippedItems;


	UItemBase* lookForFirstItem(FString &name);

	TArray<UItemBase*> lookForItems(FString &name);

	UMainInventoryWidget* mainInventory;

	TArray<UItemCategoryWidget*> categories;

	UItemBase* selectedItem;

	ItemCategory currentCategory = ItemCategory::ITEM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UDataTable* categoryTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UItemCategoryWidget> categoryTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UMainInventoryWidget> mainInventoryTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UItemWidget> itemTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TSubclassOf<class AItemBaseActor> itemBaseTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UDataTable* standardItemTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UItemDetailWidget* itemDetails;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UItemDetailWidget> itemDetailTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TArray<TSubclassOf<class UItemBase>> beginPlayEquipment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TArray<FString> beginPlayStrings;






	bool addItemCreate(UItemBase* Item);
	bool addItem(UItemBase* Item, bool forceNew = false);
	bool removeItem(UItemBase* Item, int8 ammount = -1);
	UItemBase* splitItem(UItemBase* Item, float ratio);
	void dropItem(UItemBase* Item);
	bool isEnoughSpace(UItemBase* Item);

	template <typename T>
	T* createItem(FString ID);
	void print();
	void loadUI();
	void refresh();
	void setVisiblity(bool Vis);
	
	void onCategoryClicked(UDataItemButton* sender);
	void onItemButtonClicked(UDataItemButton* sender);
	void onItemButtonHovered(UDataItemButton* sender);
	void onItemButtonLeftHovered(UDataItemButton* sender);


	//Slot function
	//param 1 which equippment widget was selected  param 2 is which item has been moved
	//for equip it is which item was placed onto the slot
	//for unequip it is which item has been removed from the slot
	void equip(UEquippedItemWidget* slot, UItemBase* base);
	void moveEquip(UEquippedItemWidget* slot, UItemBase* base);
	void unEquip(UEquippedItemWidget* slot, UItemBase* base);
};

USTRUCT(BlueprintType)
struct FCharacterEquipment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
		AWeapon* weapon1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
		ABullet* weapon1Bullet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
		UItemBase* helmet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
		UItemBase* armor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
		UItemBase* backPack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
		UItemBase* boots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
		UArtifact* art1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
		UArtifact* art2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
		UArtifact* art3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
		UArtifact* art4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
		UItemBase* quick1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
		UItemBase* quick2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
		UItemBase* quick3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
		UItemBase* quick4;

};