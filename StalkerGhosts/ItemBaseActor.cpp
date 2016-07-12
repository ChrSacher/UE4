// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "ItemBaseActor.h"
#include "StalkerGhostsCharacter.h"

// Sets default values
AItemBaseActor::AItemBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	base = CreateDefaultSubobject<UItemBase>(TEXT("Item"));
	RootComponent = mesh;
}

// Called when the game starts or when spawned
void AItemBaseActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemBaseActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void  AItemBaseActor::interact(AActor* interactor)
{
	auto x = Cast<AStalkerGhostsCharacter>(interactor);
	if(x) x->currentInventory->addItem(base);
	Destroy();

}

void  AItemBaseActor::spawn(UItemBase* Base)
{
	if (!base)
	{
		this->Destroy();
		return;
	}
	base = Base;
	if(base->groundMesh) mesh->SetStaticMesh(base->groundMesh);
}