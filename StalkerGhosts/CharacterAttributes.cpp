// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "CharacterAttributes.h"


// Sets default values for this component's properties
UCharacterAttributes::UCharacterAttributes()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	
	
	// ...
}


// Called when the game starts
void UCharacterAttributes::BeginPlay()
{
	Super::BeginPlay();
	for (uint8 i = 0; i < (uint8)AttributeType::NUM; i++)
	{
		UBaseAttribute* x = NewObject<UBaseAttribute>();
		x->parent = this;
		x->type = (AttributeType)i;
		attributes.Add((AttributeType)i, x);
	}
	nullAttrib = NewObject<UBaseAttribute>();
	// ...
	
}


// Called every frame
void UCharacterAttributes::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

float UBaseAttribute::getRaw()
{
	return baseValue;
}
void UBaseAttribute::setRaw(float x)
{
	baseValue = x;
	calculate();
}
void  UBaseAttribute::addRaw(float x)
{
	baseValue += x;
	calculate();
}
void UBaseAttribute::calculate()
{
	float add = 0;
	float mul = 1;
	for (auto& x : currentBuffs)
	{
		if (x->type == BuffType::ADDITIVE)
		{
			add += x->value;
		}
		if (x->type == BuffType::MULTIPLICATIVE)
		{
			mul += x->value;
		}
	}
	finalValue = (baseValue + add) * mul;

}
float UBaseAttribute::getFinal()
{
	return finalValue;
}

void UBaseAttribute::addBuff(UBuff* buff)
{
	if (!buff) return;
	currentBuffs.Add(buff);
	buff->onStart(parent);
	if (buff->timeBetweenTicks > 0)
	{
		buff->tickDelegate = FTimerDelegate::CreateUObject(buff, &UBuff::onTick, parent);
		GetWorld()->GetTimerManager().SetTimer(buff->tickHandle, buff->tickDelegate, buff->timeBetweenTicks, true);
	}
	if (buff->timeBetweenTicks)
	{
		buff->destroyDelegate = FTimerDelegate::CreateUObject(buff, &UBuff::onEnd, parent);
		GetWorld()->GetTimerManager().SetTimer(buff->destroyHandle, buff->destroyDelegate, buff->lifeTime, false);
	}
	calculate();
}
void UBaseAttribute::removeBuff(UBuff* buff)
{
	if (!buff) return;
	currentBuffs.Remove(buff);
	buff->onEnd(parent);
	GetWorld()->GetTimerManager().ClearTimer(buff->destroyHandle);
	GetWorld()->GetTimerManager().ClearTimer(buff->tickHandle);
	calculate();
}

UBaseAttribute* UCharacterAttributes::getAttrib(AttributeType w)
{
	auto y = attributes.Find(w);
	if(y) return *y;
	return nullAttrib;
}
void UCharacterAttributes::addBuff(UBuff* buff)
{
	getAttrib(buff->typeAffected)->addBuff(buff);
}

void UCharacterAttributes::removeBuff(UBuff* buff)
{
	getAttrib(buff->typeAffected)->removeBuff(buff);
}
UBaseAttribute::UBaseAttribute() {}
UBaseAttribute::~UBaseAttribute() {}