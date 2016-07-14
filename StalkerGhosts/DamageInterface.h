// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Actor.h"
#include "Bullet.h"
#include "Grenade.h"
#include "DamageComponent.h"
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
	virtual void doDamage(float damage,DamageBodyPart BodyPart, EDamageType type, FVector veloc = FVector(), FVector location = FVector()) {};
	virtual void startDamage(FString bonename, ABullet* causer) {}

	virtual void takeGrenadeDamage(AGrenade* Causer) {}
	virtual void startShrapnelDamage(FString bonename, AGrenade* causer) {};
}; 
