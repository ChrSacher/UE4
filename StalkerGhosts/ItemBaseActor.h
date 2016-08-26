// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "InteractInterface.h"
#include "ItemBase.h"
#include "ItemBaseActor.generated.h"

UCLASS()
class STALKERGHOSTS_API AItemBaseActor : public AActor , public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBaseActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void interact(AActor* interactor) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UItemBase* base;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		FInventoryItemInitializer initItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UBoxComponent* textCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UTextRenderComponent* text;
		void spawn(UItemBase* Base);
	UFUNCTION(BlueprintCallable, Category = "Event")
		void onActorEnter(class AActor* bActor, class AActor* cActor);
	UFUNCTION(BlueprintCallable, Category = "Event")
		void onActorLeave(class AActor* bActor, class AActor* cActor);

	void destroythis();
	FTimerHandle destroyHandle;
	AActor* activeActor;
};
