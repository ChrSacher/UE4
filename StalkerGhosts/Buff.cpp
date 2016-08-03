// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "Buff.h"

UBuff::UBuff()
{
}

UBuff::~UBuff()
{
	
}
void UBuff::load(FBuffInit& init)
{
	typeAffected = init.typeAffected;
	type = init.type;
	timeBetweenTicks = init.timeBetweenTicks;
	lifeTime = init.lifeTime;
	value = init.value;
};

void UBuff::onStart(UCharacterAttributes* attributes)
{

}


void UBuff::onTick(UCharacterAttributes* attributes)
{

}


void UBuff::onEnd(UCharacterAttributes* attributes)
{

}