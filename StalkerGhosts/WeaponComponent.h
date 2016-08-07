// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <map>
#include "ItemBase.h"
#include "Bullet.h"
#include "Components/ActorComponent.h"
#include "DataTables.h"
#include "Weapon.h"
#include "WeaponComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )

class STALKERGHOSTS_API UWeaponComponent : public UActorComponent
{
	
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		

	//std::map<std::string, UMagazineComponent> compatibleMagazines;


	

	
	UPROPERTY(EditAnywhere, Category = Weapon)
		AWeapon* weapon;

	
};
