// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Character.h"
#include "DamageEnum.h"
#include "Buff.generated.h"
/**
 * 
 */

UENUM(BlueprintType)
enum class AttributeType :uint8 //this is used for BUFFS and stuff
{
	ENDURANCE,
	AGILITY,
	INTELLIGENCE,
	CHARISMA,
	STRENGTH,
	LUCK,
	PERCEPTION,
	SPRINTSPEED,
	SPRINTCOST,
	JOGSPEED,
	WALKSPEED,
	CROUCHSPEED,
	PRONESPEED,
	STAMINA,
	MAXSTAMINA,
	STAMINACOST,
	STAMINAREGEN,
	HEALTH,
	MAXHEALTH,
	NUM, //do not remove
	NONE //so it doesn't have affect pretty much also do not remove

};


class UCharacterAttributes;
UENUM(BlueprintType)
enum class BuffType :uint8
{
	HARD, //use for damaging Effects and stuff
	ADDITIVE,
	MULTIPLICATIVE,
};

UCLASS(BlueprintType)
class STALKERGHOSTS_API UBuff : public UObject
{
	GENERATED_BODY()
public:
	UBuff();
	~UBuff();

public:
	FTimerHandle tickHandle;
	FTimerDelegate tickDelegate;

	FTimerHandle destroyHandle;
	FTimerDelegate destroyDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		AttributeType typeAffected = AttributeType::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		BuffType type = BuffType::ADDITIVE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	float timeBetweenTicks = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	float lifeTime = 60;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	float value = 1;

	UFUNCTION(BlueprintCallable, Category = "Event")
		virtual void onStart(UCharacterAttributes* attributes);
	UFUNCTION(BlueprintCallable, Category = "Event")
		virtual void onTick(UCharacterAttributes* attributes);
	UFUNCTION(BlueprintCallable, Category = "Event")
		virtual void onEnd(UCharacterAttributes* attributes);
};
