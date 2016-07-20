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
		int32 endurance = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		int32 agility = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		int32 intelligence = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		int32 charisma = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		int32 strength = 5;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		int32 luck = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		int32 perception = 5;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		float sprintSpeed = 800;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		float jogSpeed = 400;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		float walkSpeed = 200;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		float currentSpeed = 200;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		float stamina = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool isStaminaRegenerable = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		float maxStamina = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		float sprintCost = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		int32 health = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		int32 maxHealth = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
		float staminaRegen = 0.5f;
};
