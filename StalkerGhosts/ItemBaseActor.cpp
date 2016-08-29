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
	text = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	text->SetupAttachment(mesh);
	textCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("textCollision"));
	textCollision->SetupAttachment(mesh);
	RootComponent = mesh;
}

// Called when the game starts or when spawned
void AItemBaseActor::BeginPlay()
{
	Super::BeginPlay();
	if (initItem.item)
	{
		base = DuplicateObject<UItemBase>(Cast<UItemBase>(initItem.item->GetDefaultObject()), NULL);
		if (!base)
		{
			Destroy();
			return;
		}
		base->ammount = initItem.ammount;
		base->initialize(GetWorld());
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(destroyHandle, this, &AItemBaseActor::destroythis, 1.0f,false);
	}
	if (base) if (mesh) if (base->groundMesh) mesh->SetStaticMesh(base->groundMesh);
	OnActorBeginOverlap.AddDynamic(this, &AItemBaseActor::onActorEnter);
	OnActorEndOverlap.AddDynamic(this, &AItemBaseActor::onActorLeave);
	text->SetHiddenInGame(true);
}
void AItemBaseActor::destroythis()
{
	Destroy();
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
	
	base = Base;
	if (!base)
	{
		Destroy();
		return;
	}
	GetWorld()->GetTimerManager().ClearTimer(destroyHandle);
	if (base->groundMesh) mesh->SetStaticMesh(base->groundMesh);
}
//this,overlapped
void AItemBaseActor::onActorEnter(class AActor* bActor, class AActor* cActor)
{
	
	AStalkerGhostsCharacter* x = Cast<AStalkerGhostsCharacter>(cActor);
	if (!x) return;
	APlayerController* y = Cast<APlayerController>(x->GetController());
	if (y)
	{
		text->SetHiddenInGame(false);
		text->SetText(FText::FromString(base->itemIdentifier));
		activeActor = cActor;
	}
	return;
}

void  AItemBaseActor::onActorLeave(class AActor* bActor, class AActor* cActor)
{
	if (cActor == activeActor || !activeActor)
	{
		text->SetHiddenInGame(true);
	}
	activeActor = NULL;
	return;
}