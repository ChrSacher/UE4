// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#pragma once
#include "GameFramework/Actor.h"
#include "Bullet.h"
#include "DamageEnum.h"
#include "DamageInterface.generated.h"


UINTERFACE(MinimalAPI)
class UDamageInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
public:

};

class IDamageInterface
{
	GENERATED_IINTERFACE_BODY()
public:
	virtual void doDamage(DamageBodyPart BodyPart,ABullet* Cause) {};
	
};
