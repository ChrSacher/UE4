// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "DamageEnum.h"
#include "Grenade.h"
#include "Bullet.h"
#include "DamageComponent.generated.h"

class UArmorItem;
USTRUCT(Blueprintable, BlueprintType)
struct STALKERGHOSTS_API FDamageModifierStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		DamageBodyPart type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		float modifierValue;
};

USTRUCT(Blueprintable, BlueprintType)
struct STALKERGHOSTS_API FBoneBodyPartStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		TArray<FString> boneNames;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		DamageBodyPart type;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STALKERGHOSTS_API UDamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDamageComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		UPhysicsAsset* physicsAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Damage)
		TArray< float> armorValues;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Damage)
		TArray<FDamageModifierStruct> damageValues;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Damage)
		TArray<FBoneBodyPartStruct> boneBodyParts;
	UPROPERTY()
		TMap<FString,DamageBodyPart> boneBodyPartMap;
	UPROPERTY()
		TMap<DamageBodyPart, float> damageModifierMap;

public:
	void setup();
	UFUNCTION(BlueprintCallable, Category = "Event")
		float damageAmmount(DamageBodyPart BodyPart, ABullet* bullet);
		float damageAmmount(DamageBodyPart BodyPart, float damage);

	UFUNCTION(BlueprintCallable, Category = "Event")
		float calculateGrenadeDamage(AGrenade* grenade);
		
	UFUNCTION(BlueprintCallable, Category = "Event")
		DamageBodyPart getDamagedBodyPart(FString bonename);

	void addArmor(UArmorItem* armor);
	void removeArmor(UArmorItem* armor);
};
