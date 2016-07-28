// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "DamageEnum.h"
#include "Grenade.h"
#include "Bullet.h"
#include "DamageComponent.generated.h"


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		UPhysicsAsset* physicsAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		TArray<FString> boneNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		TArray<DamageBodyPart> damageTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
		TArray<float> damageModifiers;

		TMap<FString,DamageBodyPart> boneBodyPartMap;

		
			TMap<DamageBodyPart, float> damageModifierMap;

	
	void setup();
	UFUNCTION(BlueprintCallable, Category = "Event")
		float damageAmmount(DamageBodyPart BodyPart, ABullet* bullet);
		float damageAmmount(DamageBodyPart BodyPart, float damage);

	UFUNCTION(BlueprintCallable, Category = "Event")
		float calculateGrenadeDamage(AGrenade* grenade);

	UFUNCTION(BlueprintCallable, Category = "Event")
		DamageBodyPart getDamagedBodyPart(FString bonename);
};
