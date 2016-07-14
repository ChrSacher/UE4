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
	NON = 0,
	FOOT = 1,
	LOWERLEG = 2,
	UPPERLEG = 3,
	PELVIS = 4,
	STOMACH = 5,
	CHEST = 6,
	UPPERARM = 7,
	LOWERARM = 8,
	HAND = 9,
	THROAT = 10,
	HEAD = 11,
	NUM = 12
};

