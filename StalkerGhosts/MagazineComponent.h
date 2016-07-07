// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Bullet.h"
#include "ItemBase.h"
#include "Components/ActorComponent.h"
#include "MagazineComponent.generated.h"
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STALKERGHOSTS_API UMagazineComponent : public UItemBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMagazineComponent();
	~UMagazineComponent() {}
	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UPROPERTY(EditAnywhere, Category = Magazine)
		int8 ammoCapacity = 30;
	UPROPERTY(EditAnywhere, Category = Magazine)
		int8 currentAmmoCount = 30;

	inline static bool compareCap(const UMagazineComponent& first, const UMagazineComponent& second)
	{
		return first.ammoCapacity < second.ammoCapacity;
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
		TSubclassOf<class ABullet> bullet;

	bool fireBullet(FVector SpawnLocation, FRotator SpawnRotation);
};
