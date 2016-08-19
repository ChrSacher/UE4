// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Actor.h"
#include "Bullet.h"
#include "Grenade.h"
#include "DamageEnum.h"
#include "DamageComponent.h"
#include "DamageInterface.generated.h"



USTRUCT(Blueprintable)
struct FMainDamageEvent :public FDamageEvent
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		FHitResult hit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		EDamageType type;
	virtual bool IsOfType(int32 InID) const { return FMainDamageEvent::ClassID == InID; };
	static const int32 ClassID = 5;
};
USTRUCT(Blueprintable)
struct FSuppresionDamageEvent :public FDamageEvent
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		FHitResult hit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		float suppresionAmmount = 10;
	virtual bool IsOfType(int32 InID) const { return FSuppresionDamageEvent::ClassID == InID; };
	static const int32 ClassID = 6;
};