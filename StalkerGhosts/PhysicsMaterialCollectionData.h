// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "PhysicsMaterialCollectionData.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct STALKERGHOSTS_API FPhysicsMaterialSounds
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		TEnumAsByte<EPhysicalSurface> surfaceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		TArray<USoundBase*> sounds;
};

USTRUCT(Blueprintable, BlueprintType)
struct STALKERGHOSTS_API FPhysicsMaterialPenetration
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		TEnumAsByte<EPhysicalSurface> surfaceType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		float velocityLossPerCM = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		float maxSpreadX = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		float maxSpreadY = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool allowPenetration = true;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STALKERGHOSTS_API UPhysicsMaterialCollectionData : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPhysicsMaterialCollectionData();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	//phyisics material 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TArray<FPhysicsMaterialSounds> PhysicsMaterialSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		TArray<FPhysicsMaterialPenetration> penetrationData;
	//COULD BE OPTIMIZED INTO A MAP
	UFUNCTION(BlueprintCallable, Category = "Event")
		USoundBase* getPhysicsSound(TEnumAsByte<EPhysicalSurface> type, int32 index = -1);
	

	UFUNCTION(BlueprintCallable, Category = "Event")
		FPhysicsMaterialPenetration& getPhysicsPenetrationData(TEnumAsByte<EPhysicalSurface> type);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FPhysicsMaterialPenetration defaultPenetrationData;
};
