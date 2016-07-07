// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "ItemBase.h"

UItemBase::UItemBase()
{
	
}

UItemBase::~UItemBase()
{

}

bool UItemBase::equals(UItemBase* other)
{
	return name == other->name;
}

void UItemBase::BeginPlay()
{
	Super::BeginPlay();

	

}