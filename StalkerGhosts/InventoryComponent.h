#pragma once
#include "ItemBase.h"
#include "Components/ActorComponent.h"
#include "MainInventoryWidget.h"
#include "ItemCategoryWidget.h"
#include "DataTables.h"
#include "InventoryComponent.generated.h"

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
	
	
	TMap<ItemCategory, TMap<AItemBase*,AItemBase*>> items;

	AItemBase* lookForFirstItem(FString &name);

	TArray<AItemBase*> lookForItems(FString &name);

	UMainInventoryWidget* mainInventory;

	TArray<UItemCategoryWidget*> categories;

	AItemBase* selectedItem;

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
		UDataTable* itemTable;

	bool addItemCreate(AItemBase* Item);
	bool addItem(AItemBase* Item, bool forceNew = false);
	AItemBase* addItem(FString& ID, bool forceNew = false);
	bool removeItem(AItemBase* Item, int8 ammount = -1);
	AItemBase* splitItem(AItemBase* Item, float ratio);
	void dropItem(AItemBase* Item);
	bool isEnoughSpace(AItemBase* Item);

	void print();
	void loadUI();
	void refresh();
	void setVisiblity(bool Vis);
	
	void onCategoryClicked(UDataItemButton* sender);
	void onItemButtonClicked(UDataItemButton* sender);
	void onItemButtonHovered(UDataItemButton* sender);
	void onItemButtonLeftHovered(UDataItemButton* sender);
};

