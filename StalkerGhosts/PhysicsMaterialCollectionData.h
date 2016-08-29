// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "PhysicsMaterialCollectionData.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct STALKERGHOSTS_API FPhysicsMaterialPenetration
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		float velocityLossPerCM = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		float maxSpreadX = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		float maxSpreadY = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		bool allowPenetration = true;

};

USTRUCT(Blueprintable, BlueprintType)
struct STALKERGHOSTS_API FPhysicsMaterialDecal
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		TArray<UMaterial*> decals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		float lifeTime = 5.0f; //should be kept low

};

USTRUCT(Blueprintable, BlueprintType)
struct STALKERGHOSTS_API FPhysicsMaterialSound
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		TArray<USoundBase*> sounds;

};

USTRUCT(Blueprintable, BlueprintType)
struct STALKERGHOSTS_API FPhysicsMaterialCollection
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		TEnumAsByte<EPhysicalSurface> surfaceType;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		FPhysicsMaterialPenetration penetrationData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		FPhysicsMaterialDecal decals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		FPhysicsMaterialSound sounds;




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
		TArray<FPhysicsMaterialCollection> PhysicsMaterial;
	
	//COULD BE OPTIMIZED INTO A MAP
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FPhysicsMaterialCollection defaultMaterial;

	UFUNCTION(BlueprintCallable, Category = "Event")
		USoundBase* getPhysicsSound(TEnumAsByte<EPhysicalSurface> type, int32 index = -1);

	UFUNCTION(BlueprintCallable, Category = "Event")
		UMaterial* getPhysicsDecal(TEnumAsByte<EPhysicalSurface> type, int32 index = -1);

	UFUNCTION(BlueprintCallable, Category = "Event")
		FPhysicsMaterialCollection&  getCollection(TEnumAsByte<EPhysicalSurface> type);

	UFUNCTION(BlueprintCallable, Category = "Event")
		FPhysicsMaterialPenetration& getPhysicsPenetrationData(TEnumAsByte<EPhysicalSurface> type);
};
