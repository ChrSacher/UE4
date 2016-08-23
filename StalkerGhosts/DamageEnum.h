// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Actor.h"
#include "DamageEnum.generated.h"

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	NON,
	BULLET,
	SHRAPNEL,
	EXPLOSION,
	BLUNT,
	SLICE,
	STAB,
	RADIATION,
	FALLING,
	NUM 
};

UENUM(BlueprintType)
enum class DamageBodyPart : uint8
{
	NON ,
	FOOT ,
	LOWERLEG ,
	UPPERLEG ,
	PELVIS ,
	STOMACH ,
	CHEST ,
	LOWERBACK,
	UPPERBACK,
	UPPERARM ,
	LOWERARM ,
	
	HAND ,
	THROAT ,
	HEAD ,
	ALL, //is used for everything which doesn't have a kind of attack angle like radiation
	NUM 
};

