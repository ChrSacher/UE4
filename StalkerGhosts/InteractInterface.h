// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Actor.h"
#include "InteractInterface.generated.h"

	UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IInteractInterface
{
	GENERATED_IINTERFACE_BODY()

		virtual FString ToString();
		virtual void interact(AActor* interactor);
		virtual void use(AActor* interactor);
};

