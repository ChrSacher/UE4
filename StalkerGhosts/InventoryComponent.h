#pragma once
#include "ItemBase.h"
#include "Components/ActorComponent.h"

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

	TMap<ItemCategory,TArray<UItemBase*>> items;
	TMap<UItemBase*, UUserWidget*> UI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> inv;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> invItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> contextMenu;

	//Null is emoty spot
	UPROPERTY(EditAnywhere, Category = Inventory)
		int8 width = 10; //should stay 10
	UPROPERTY(EditAnywhere, Category = Inventory)
		int8 height = 10 ;

	UPROPERTY(EditAnywhere, Category = Inventory)
		int8 maxWeight;

	UPROPERTY(EditAnywhere, Category = Inventory)
		int8 currentWeight;
	
		bool addItem(UItemBase* Item);
	

	
		bool removeItem(UItemBase* Item, uint8 ammount = -1);
		UItemBase* splitItem(UItemBase* Item,float ratio);
	

	
		bool isEnoughSpace(UItemBase* Item);
	
		void print();
		void loadUI();
		void refresh() {};
		void setVisiblity(bool Vis) {};

		UItemBase* lookForFirstItem(FString &name);
		TArray<UItemBase*> lookForItems(FString &name);

		UUserWidget* mainInventory;
		UUserWidget* categorySlot;
		TArray<UUserWidget*> categories;
		UItemBase* selectedItem;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
			TSubclassOf<UUserWidget> categoryTemplate;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
			TSubclassOf<UUserWidget> mainInventoryTemplate;

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
			TSubclassOf<UUserWidget> itemTemplate;
};

