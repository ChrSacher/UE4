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
	
	UPROPERTY(EditAnywhere, Category = Weapon)
		bool isReloading = false;

	UPROPERTY(EditAnywhere, Category = Weapon)
		AWeapon* weapon;

	void loadWeapon(UWeaponItem* wep);
	void loadWeapon(AWeapon* ID);
	bool loadMag(UBulletItem* ID);
	void unloadMag();
	void playSound(FVector place);
	int32 getAmmoCount();
	void playEmptySound(FVector place);
	void startReload();
	void removeWeapon();
	bool reload(UBulletItem* bullet);
	void endReload();
	bool Fire(FVector SpawnLocation, FRotator SpawnRotation);
	UBulletItem* getLoadedMag();
};
