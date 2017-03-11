#pragma once
#include "ItemBase.h"
#include "Components/ActorComponent.h"
#include "MainInventoryWidget.h"
#include "ItemCategoryWidget.h"
#include "DataTables.h"
#include "EquippedItemWidget.h"
#include "Weapon.h"
#include "InventoryComponent.generated.h"


UENUM(BlueprintType)
enum class InventoryState: uint8
{
	INVENTORY,
	TRANSFER,
	TRADE
};

USTRUCT(Blueprintable, BlueprintType)
struct STALKERGHOSTS_API FEquipmentInit
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		TSubclassOf<UItemBase> item;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Damage)
		EquipmentInformation slot;
};

class UInventoryComponent;

UCLASS(BlueprintType)
class UCharacterEquipment : public UObject
{
	DECLARE_DELEGATE_TwoParams(EquipDelegate, UItemBase*, EquipmentInformation);
	
	GENERATED_BODY()
		UCharacterEquipment();
		~UCharacterEquipment();
protected:
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
		TArray<UItemBase*> equipment;
public:
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
		TArray<FEquipmentInit> initEquipment;

	//should be called once delegate is setup, will initialiaze standard Equipment
	UFUNCTION(BlueprintCallable, Category = "Event")
		void initializeStartEquipment();

		/*
		*	equips the Item and calls Delegate
		*	param slot   in which place the item is being equipped
		#	param item	item to be equipped
		*
		*/	
	UFUNCTION(BlueprintCallable, Category = "Event")
		void equipItem(EquipmentInformation slot, UItemBase* item);
	/*
	*	look through equipment for item and unequip if exists
	*	param item   item to be searched for
	*	return item which was uneqiupped
	*
	*/
	UFUNCTION(BlueprintCallable, Category = "Event")
		UItemBase* unequipItemByPointer(UItemBase* item);

	/*
	*	unequips the Item and calls Delegate
	*	param slot   in which place the item is being unequipped
	*	return item which was uneqiupped
	*
	*/
	UFUNCTION(BlueprintCallable, Category = "Event")
		UItemBase* unequipItem(EquipmentInformation slot);

	/*
	*	return all equipped Items can be used for inventory when dead
	*	
	*	return all equipped items
	*
	*/
	UFUNCTION(BlueprintCallable, Category = "Event")
		TArray<UItemBase*> dumbItems();


	/*
	*	checks if a item is equippable by casting to the appropiate type
	*
	*	return true if item can be equipped
	*
	*/
	UFUNCTION(BlueprintCallable, Category = "Event")
		bool isItemEquippable(EquipmentInformation slot,UItemBase* Item);

	UPROPERTY(EditAnywhere, Category = Inventory)
		UEquipmentWidget* equipmentWidget;

	UPROPERTY(EditAnywhere, Category = Inventory)
		TSubclassOf<UEquipmentWidget> equipmentWidgetTemplate;

	UPROPERTY(EditAnywhere, Category = Inventory)
	UInventoryComponent* attachedInventory;

		EquipDelegate equipDelegate;
		
		//not needed 
		void setupUI();

		EquipDelegate unEquipDelegate;

		template <typename T>
		T* getItem(EquipmentInformation slot);

		/*

		* return weight of all items equipped
		*/
	UFUNCTION(BlueprintCallable, Category = "Event")
		float getWeight();


};
USTRUCT()
struct FItemMap
{
	GENERATED_BODY()
public:
	UPROPERTY()
		TMap<UItemBase*, UItemBase*> map;
};

class AWeapon;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STALKERGHOSTS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();
	DECLARE_DELEGATE_OneParam(WeightChangedDelegate, float);
	WeightChangedDelegate OnWeightChanged;



	// Called when the game starts
	virtual void BeginPlay() override;
	// Called every frameini
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UPROPERTY(EditAnywhere, Category = Inventory)
		bool requiresUI = false;
	UPROPERTY(EditAnywhere, Category = Inventory)
		float maxWeight = 60;

	UPROPERTY(EditAnywhere, Category = Inventory)
		float currentWeight = 0;

	UPROPERTY(EditAnywhere, Category = Inventory)
	TMap<ItemCategory, FItemMap > items;



	UPROPERTY(EditAnywhere, Category = Inventory)
		UMainInventoryWidget* mainInventory;

	UPROPERTY(EditAnywhere, Category = Inventory)
		UItemBase* selectedItem;
	UPROPERTY(EditAnywhere, Category = Inventory)
		ItemCategory currentCategory = ItemCategory::ITEM;
	UPROPERTY(EditAnywhere, Category = Inventory)
		ItemCategory otherCurrentCategory = ItemCategory::ITEM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UMainInventoryWidget> mainInventoryTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UItemWidget> itemTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TSubclassOf<class AItemBaseActor> itemBaseTemplate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		FTimerHandle transferRangeTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		FTimerHandle itemDetailsTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UDataTable* standardItemTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TArray<FInventoryItemInitializer> beginPlayEquipment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TArray<FString> beginPlayStrings;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UCharacterEquipment* equipment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UInventoryComponent* otherInventoryForTransfering;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TArray<UItemBase*> itemsToSell;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TArray<UItemBase*> itemsToBuy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UDataItemButton* hoveredButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		InventoryState state = InventoryState::INVENTORY;

	//inventoryfuncs

	/*
	param name of item
	returns first Item with name
	*/
	UFUNCTION(BlueprintCallable, Category = "Event")
		UItemBase* lookForFirstItem(FString &name);

	/*
	param name of item
	returns all Item with name
	*/
	UFUNCTION(BlueprintCallable, Category = "Event")
		TArray<UItemBase*> lookForItems(FString name);

	//helper func for creating items
	UFUNCTION(BlueprintCallable, Category = "Event")
		bool addItemCreate(UItemBase* Item);

	/*
	param name itemName
	param float ammount
	param forceNew will add the item withut splitting
	*/
	UFUNCTION(BlueprintCallable, Category = "Event")
		bool addItemByClass(TSubclassOf<UItemBase> item,int32 ammount, bool forceNew = false);
	/*
	param name itemName
	param float ammount
	param forceNew will add the item withut splitting
	*/
	UFUNCTION(BlueprintCallable, Category = "Event")
		bool addItemByName(FString name, int32 ammount, bool forceNew = false);

		/*param Item item to be added
		param forceNew will add the item withut splitting
		*/
	UFUNCTION(BlueprintCallable, Category = "Event")
		bool addItem(UItemBase* Item, bool forceNew = false);

	/*
	removes ammount from Item if -1 all of it
	param Item item to be removed
	param ammount
	*/
	UFUNCTION(BlueprintCallable, Category = "Event")
		bool removeItem(UItemBase* Item, int32 ammount = -1);

	/*
	removes ammount items with name itemName
	return all Items with ammount 0
	*/
	UFUNCTION(BlueprintCallable, Category = "Event")
		TArray<UItemBase*>  removeItems(FString& itemName, int32 ammount = -1);

	/*
	removes ammount items with name itemName
	creates new items
	return new items with ammount overall
	*/
	UFUNCTION(BlueprintCallable, Category = "Event")
		TArray<UItemBase*>  getItemsByAmmount(FString itemName, int32 ammount);
	UFUNCTION(BlueprintCallable, Category = "Event")
		void combineItems(TArray<UItemBase*> &item);
	UFUNCTION(BlueprintCallable, Category = "Event")
		int32 getItemCount(FString& itemName);

	UFUNCTION(BlueprintCallable, Category = "Event")
		UItemBase* splitItem(UItemBase* Item, float ratio);
	UFUNCTION(BlueprintCallable, Category = "Event")
		void dropItem(UItemBase* Item);
	UFUNCTION(BlueprintCallable, Category = "Event")
		bool isEnoughSpace(UItemBase* Item);
	UFUNCTION(BlueprintCallable, Category = "Event")
		bool isItemInInvenotry(UItemBase* Item);
	UFUNCTION(BlueprintCallable, Category = "Event")
		void calculateWeight();
	UFUNCTION(BlueprintCallable, Category = "Event")
		float getWeight();
	UFUNCTION(BlueprintCallable, Category = "Event")
		void loadCategories();
	UFUNCTION(BlueprintCallable, Category = "Event")
		bool isInRangeOfOtherInventory(UInventoryComponent* otherInventory);
	UFUNCTION(BlueprintCallable, Category = "Event")
		void checkRangeOnOtherInventory();
	void loadItemWidget(UItemBase* item, UItemScrollBoxWidget* scrollBox);
	
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
	UFUNCTION(BlueprintCallable, Category = "Event")
		void showInventory();
	UFUNCTION(BlueprintCallable, Category = "Event")
		void hideInventory();
	UFUNCTION(BlueprintCallable, Category = "Event")
		bool isVisible();


	UFUNCTION(BlueprintCallable, Category = "Event")
		void onCategoryClicked(UItemCategoryWidget* sender);

	UFUNCTION(BlueprintCallable, Category = "Event")
		void onOtherCategoryClicked(UItemCategoryWidget* sender);

	UFUNCTION(BlueprintCallable, Category = "Event")
		void onItemButtonClicked(UItemWidget* sender);

	UFUNCTION(BlueprintCallable, Category = "Event")
		void onItemButtonHovered(UItemWidget* sender);

	UFUNCTION(BlueprintCallable, Category = "Event")
		void onItemButtonLeftHovered(UItemWidget* sender);

	UFUNCTION(BlueprintCallable, Category = "Event")
		void onItemButtonHoveredTimer();


	UFUNCTION(BlueprintCallable, Category = "Event")
		void closeTransferWindow();
	UFUNCTION(BlueprintCallable, Category = "Event")
		void openTransferWindow(UInventoryComponent* otherInventory);
	UFUNCTION(BlueprintCallable, Category = "Event")
		void closeTradeWindow();
	UFUNCTION(BlueprintCallable, Category = "Event")
		void openTradeWindow(UInventoryComponent* otherInventory);
	UFUNCTION(BlueprintCallable, Category = "Event")
		void tradeItems();
	UFUNCTION(BlueprintCallable, Category = "Event")
		void resetItems();
	UFUNCTION(BlueprintCallable, Category = "Event")
		void sellItem(UItemBase* Item);
	UFUNCTION(BlueprintCallable, Category = "Event")
		void buyItem(UItemBase* Item);
	UFUNCTION(BlueprintCallable, Category = "Event")
		void unsellItem(UItemBase* item);
	UFUNCTION(BlueprintCallable, Category = "Event")
		void unBuyItem(UItemBase* Item);
	//Slot function
	//param 1 which equippment widget was selected  param 2 is which item has been moved
	//for equip it is which item was placed onto the slot
	//for unequip it is which item has been removed from the slot
	//move is for unequipping item and then moving it to the other inventory
	UFUNCTION(BlueprintCallable, Category = "Event")
		void equip(UEquippedItemWidget* slot, UItemBase* base);
	UFUNCTION(BlueprintCallable, Category = "Event")
		void moveItem(UItemWidget* slot, UItemBase* base, UItemScrollBoxWidget* sender);
	UFUNCTION(BlueprintCallable, Category = "Event")
		void unEquip(UEquippedItemWidget* slot, UItemBase* base, UItemScrollBoxWidget* sender);

	UFUNCTION(BlueprintCallable, Category = "Event")
		void attachWeaponAttachment(UWeaponAttachmentWidget* slot, UItemBase* base);
	UFUNCTION(BlueprintCallable, Category = "Event")
		void detachWeaponAttachment(UWeaponAttachmentWidget* slot, UItemBase* base, UItemScrollBoxWidget* sender);

};



UCLASS()
class STALKERGHOSTS_API UCraftingMenuWidget : public UUserWidget
{
	GENERATED_BODY()
		DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCraftingMenuWidgetDelegate, UCraftingMenuWidget*, craftingWidget);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Crafting)
		UButton* closeButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Crafting)
		UButton* craftButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Crafting)
		UBorder* border;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Crafting)
		UScrollBox* recipeScrollBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Crafting)
		FCraftingTableRow currentRow;

	UFUNCTION()
		void onCloseButtonClicked()
		{
			onCloseClicked.Broadcast(this);
		}
	UFUNCTION()
		void onCraftButtonClicked()
		{
			onCraftClicked.Broadcast(this);
		}

	UPROPERTY(BlueprintAssignable)
		FCraftingMenuWidgetDelegate onCloseClicked;

	UPROPERTY(BlueprintAssignable)
		FCraftingMenuWidgetDelegate onCraftClicked;
};

class URecipe;
UCLASS()
class STALKERGHOSTS_API URecipeWidget : public UUserWidget
{
	GENERATED_BODY()
		DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRecipeClickedDelegate, URecipeWidget*, recipeWidget);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Crafting)
		UButton* button;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Crafting)
		URecipe* associatedRecipe;

	
	UFUNCTION()
		void load(FCraftingTableRow& row)
		{

		}

	UFUNCTION()
		void onButtonClicked()
		{
			onClicked.Broadcast(this);
		}
	UFUNCTION()
		void onButtonHovered()
		{
			onHovered.Broadcast(this);
		}
	UFUNCTION()
		void onButtonUnHovered()
		{
			onUnHovered.Broadcast(this);
		}

	UPROPERTY(BlueprintAssignable)
		FRecipeClickedDelegate onClicked;

	UPROPERTY(BlueprintAssignable)
		FRecipeClickedDelegate onHovered;

	UPROPERTY(BlueprintAssignable)
		FRecipeClickedDelegate onUnHovered;

};
UCLASS(Blueprintable, BlueprintType)
class URecipe : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recipe)
		FString displayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recipe)
		FString identifierName;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recipe)
		TArray< FCraftingItemAmmount> requiredItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recipe)
		TSubclassOf<UItemBase> finalProduct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recipe)
		float ammount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recipe)
		URecipeWidget* associatedRecipeWidget;

};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class STALKERGHOSTS_API UCraftingSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MoveAround)
		URecipe* emptyRecipe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MoveAround)
		URecipe* selectedRecipe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MoveAround)
		UInventoryComponent* attachedInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MoveAround)
		UCraftingMenuWidget* craftingMenuWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Crafting)
		UDataTable* recipeList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Crafting)
		TArray<URecipe*> recipes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Crafting)
		TSubclassOf<URecipeWidget> recipeTemplate;
	UFUNCTION(BlueprintCallable, Category = "Event")
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable, Category = "Event")
	void addRecipe(URecipe* recipe);
	UFUNCTION(BlueprintCallable, Category = "Event")
	void loadRecipe(URecipe* recipe);
	UFUNCTION(BlueprintCallable, Category = "Event")
	void bindRecipe(URecipe* recipe);
	UFUNCTION(BlueprintCallable, Category = "Event")
	void refresh();
	UFUNCTION(BlueprintCallable, Category = "Event")
	bool hasEnoughItems();
	UFUNCTION(BlueprintCallable, Category = "Event")
	void craftItem(URecipe* recipe);
	UFUNCTION(BlueprintCallable, Category = "Event")
	void onRecipeClicked(URecipeWidget* recipeWidget);
	UFUNCTION(BlueprintCallable, Category = "Event")
	void onCloseButtonPressed();
	UFUNCTION(BlueprintCallable, Category = "Event")
	void onCraftButtonClicked();
	UFUNCTION(BlueprintCallable, Category = "Event")
	void closeCraftingMenu();
	UFUNCTION(BlueprintCallable, Category = "Event")
	void openCraftingMenu(UInventoryComponent* inventory);
};
