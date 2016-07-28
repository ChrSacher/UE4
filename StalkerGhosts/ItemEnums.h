// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class ItemCategory : uint8
{
	ALL = 0,
	WEAPON,
	AMMUNITION,
	ARTIFACT,
	NIGHTVISION,
	ARMOR ,
	BOOTS ,
	GLOVES ,
	ITEM ,
	MEDICAL ,
	FOOD ,
	GRENADES ,
	NUM 
};

UENUM(BlueprintType)
enum class SlotInformation : uint8
{
	NON,
	HELMET,
	ARMOR,
	BACKPACK,
	BOOTS,
	WEAPON1,
	WEAPON1AMMUNITION,
	ARTIFACT1,
	ARTIFACT2,
	ARTIFACT3,
	ARTIFACT4,
	QUICK1,
	QUICK2,
	QUICK3,
	QUICK4
};