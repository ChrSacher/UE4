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
	ITEM ,
	CONSUMABLE ,
	GRENADES ,
	NUM 
};

UENUM(BlueprintType)
enum class ArmorSubCategory : uint8
{
	HELMET,
	ARMOR,
	BACKPACK,
	BOOTS,
	PANTS,
	BODYARMOR,
	NUM
};

UENUM(BlueprintType)
enum class SlotInformation : uint8
{
	HELMET,
	ARMOR,
	BACKPACK,
	BOOTS,
	WEAPON1,
	PANTS,
	BODYARMOR,
	ARTIFACT1,
	ARTIFACT2,
	ARTIFACT3,
	ARTIFACT4,
	QUICK1,
	QUICK2,
	QUICK3,
	QUICK4,
	NUM,
	NON
};