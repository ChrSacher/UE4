// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "DamageEnum.h"
#include "DamageInterface.h"
#include "AreaDamageComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STALKERGHOSTS_API UAreaDamageComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAreaDamageComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GInventory)
		TArray<AActor*> actorsInsideArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GInventory)
		float damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GInventory)
		float tickTimeInSeconds = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GInventory)
		EDamageType damageType = EDamageType::EXPLOSION;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GInventory)
		USoundBase* tickSound;
	UFUNCTION(BlueprintCallable, Category = "Event")
		void addActor(AActor* actor);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = GInventory)
		bool isRunning = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GInventory)
		FTimerHandle tickHandle;
	void runTick();
	UFUNCTION(BlueprintCallable, Category = "Event")
		void removeActor(AActor* actor);


};
