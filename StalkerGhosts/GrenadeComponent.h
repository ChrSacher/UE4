// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grenade.h"
#include "ItemBase.h"
#include "GrenadeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STALKERGHOSTS_API UGrenadeComponent : public UItemBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrenadeComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grenade)
		TArray <FString> allowedGrenadesStrings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grenade)
		TArray <AGrenade*> allowedGrenades;
	
	TMap<FString, AGrenade*> allowedGrenadesMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grenade)
		FString selectedGrenade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Grenade)
		TSubclassOf<class AGrenade> bullet;

	bool throwGrenade(FVector SpawnLocation, FRotator SpawnRotation);
};
