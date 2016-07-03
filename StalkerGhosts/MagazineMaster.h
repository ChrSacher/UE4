// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Bullet.h"
#include "GameFramework/Actor.h"
#include "MagazineMaster.generated.h"

UCLASS()
class STALKERGHOSTS_API AMagazineMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagazineMaster();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	
	UPROPERTY(EditAnywhere, Category = Magazine)
		FString name = "DefaultMag";
	UPROPERTY(EditAnywhere, Category = Magazine)
		int32 ammoCapacity = 30;
	UPROPERTY(EditAnywhere, Category = Magazine)
		int32 currentAmmoCount = 30;

	UPROPERTY(EditAnywhere, Category = Magazine)
		UTexture2D* picture;

	UPROPERTY(EditAnywhere, Category = Magazine)
		TSubclassOf<ABullet> bullet;
	
};
