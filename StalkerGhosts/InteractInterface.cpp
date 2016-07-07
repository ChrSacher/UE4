// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "InteractInterface.h"

//////////////////////////////////////////////////////////////////////////
// ToStringInterface

UInteractInterface::UInteractInterface(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

//This is required for compiling, would also let you know if somehow you called
//the base event/function rather than the over-rided version
FString IInteractInterface::ToString()
{
	return "IToStringInterface::ToString()";
}