// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Actor.h"
#include "DamageEnum.generated.h"

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	NON = 0,
	BULLET = 1
};

UENUM(BlueprintType)
enum class DamageBodyPart : uint8
{
	NON = 0
};

