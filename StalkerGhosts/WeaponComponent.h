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


	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GInventory)
		FTimerHandle reloadHandle;

	

	UPROPERTY(VisibleAnywhere, Category = Weapon)
		FString loadedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
		UDataTable* bulletDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
		UDataTable* weaponDataTable;

	

	
	
	UPROPERTY(EditAnywhere, Category = Weapon)
		bool isReloading = false;

	

	UPROPERTY(EditAnywhere, Category = Weapon)
		AWeapon* weapon;

		void loadWeapon(FString &ID);
		void loadWeapon(AWeapon* ID);
	void playSound(FVector place);

	void playEmptySound(FVector place);
	void startReload();
	bool reload(int32 &bulletammount);
	void endReload();
	bool Fire(FVector SpawnLocation, FRotator SpawnRotation);

};
