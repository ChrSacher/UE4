// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "StalkerGhostsCharacter.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UStaticMeshComponent* mesh;
	
};
