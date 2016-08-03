// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Actor.h"
#include "DamageEnum.generated.h"

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	NON = 0,
	BULLET = 1,
	SHRAPNEL = 2,
	EXPLOSION = 3,
	BLUNT = 4,
	SLICE = 5,
	STAB = 6,
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
	UPPERARM ,
	LOWERARM ,
	HAND ,
	THROAT ,
	HEAD ,
	NUM 
};

