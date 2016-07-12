// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CharacterAttributes.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STALKERGHOSTS_API UCharacterAttributes : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterAttributes();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		uint8 endurance = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		uint8 agility = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		uint8 intelligence = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		uint8 charisma = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		uint8 strength = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		uint8 health = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		uint8 stamina = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		uint8 luck = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		uint8 perception = 5;
		
	
};
