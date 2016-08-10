#pragma once
#include "ItemBase.h"
#include "Components/ActorComponent.h"
#include "MainInventoryWidget.h"
#include "ItemCategoryWidget.h"
#include "DataTables.h"
#include "EquippedItemWidget.h"
#include "InventoryComponent.generated.h"




UCLASS(BlueprintType)
class UCharacterEquipment : public UObject
{
	GENERATED_BODY()
		UCharacterEquipment();
		~UCharacterEquipment();
protected:
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		TArray<UItemBase*> equipment;
public:
	UFUNCTION(BlueprintCallable, Category = "Event")
		void equipItem(SlotInformation slot, UItemBase* item);
	UFUNCTION(BlueprintCallable, Category = "Event")
		UItemBase* unequipItem(SlotInformation slot);
	
		template <typename T>
		T* getItem(SlotInformation slot);
	UFUNCTION(BlueprintCallable, Category = "Event")
		float getWeight();


};

class AWeapon;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STALKERGHOSTS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	DECLARE_DELEGATE_ThreeParams(EquipDelegate, UItemBase*, SlotInformation,bool&);
		EquipDelegate equipDelegate;
		EquipDelegate unEquipDelegate;
	// Called when the game starts
	virtual void BeginPlay() override;
	// Called every frameini
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;


	UPROPERTY(EditAnywhere, Category = Inventory)
		float maxWeight = 60;

	UPROPERTY(EditAnywhere, Category = Inventory)
		float currentWeight = 0;
	
	TMap<ItemCategory, TMap<UItemBase*,UItemBase*>> items;

	UItemBase* lookForFirstItem(FString &name);

	TArray<UItemBase*> lookForItems(FString name);
	UPROPERTY(EditAnywhere, Category = Inventory)
		UMainInventoryWidget* mainInventory;
	UPROPERTY(EditAnywhere, Category = Inventory)
		TArray<UItemCategoryWidget*> categories;
	UPROPERTY(EditAnywhere, Category = Inventory)
		UItemBase* selectedItem;
	UPROPERTY(EditAnywhere, Category = Inventory)
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UCharacterEquipment* equipment;




	//inventoryfuncs
	UFUNCTION(BlueprintCallable, Category = "Event")
	bool addItemCreate(UItemBase* Item);
	UFUNCTION(BlueprintCallable, Category = "Event")
	bool addItem(UItemBase* Item, bool forceNew = false);
	UFUNCTION(BlueprintCallable, Category = "Event")
	bool removeItem(UItemBase* Item, int32 ammount = -1);
	UFUNCTION(BlueprintCallable, Category = "Event")
	UItemBase* splitItem(UItemBase* Item, float ratio);
	UFUNCTION(BlueprintCallable, Category = "Event")
	void dropItem(UItemBase* Item);
	UFUNCTION(BlueprintCallable, Category = "Event")
	bool isEnoughSpace(UItemBase* Item);
	UFUNCTION(BlueprintCallable, Category = "Event")
	void calculateWeight();
	UFUNCTION(BlueprintCallable, Category = "Event")
	float getWeight();

	template <typename T>
	T* createItem(FString ID);
	UFUNCTION(BlueprintCallable, Category = "Event")
	void print();
	UFUNCTION(BlueprintCallable, Category = "Event")
	void loadUI();
	UFUNCTION(BlueprintCallable, Category = "Event")
	void refresh();
	UFUNCTION(BlueprintCallable, Category = "Event")
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

