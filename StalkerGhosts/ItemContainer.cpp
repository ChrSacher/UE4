// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "ItemContainer.h"
#include "StalkerGhostsCharacter.h"

// Sets default values
AItemContainer::AItemContainer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	container = CreateDefaultSubobject<UInventoryComponent>("container");
}

// Called when the game starts or when spawned
void AItemContainer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemContainer::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AItemContainer::interact(AActor* interactor)
{
	Cast<AStalkerGhostsCharacter>(interactor)->currentInventory->openTransferWindow(container);
}