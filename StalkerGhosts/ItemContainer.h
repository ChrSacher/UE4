// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "InventoryComponent.h"
#include "InteractInterface.h"
#include "ItemContainer.generated.h"

UCLASS()
class STALKERGHOSTS_API AItemContainer : public AActor ,public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemContainer();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void interact(AActor* interactor) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		 UInventoryComponent* container;
};
