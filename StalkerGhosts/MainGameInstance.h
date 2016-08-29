// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "PhysicsMaterialCollectionData.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class STALKERGHOSTS_API UStalkerMainGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		UUserWidget* mainGameWidget;

	UFUNCTION(BlueprintCallable, Category = "Event")
	void addWidgetToMain(UUserWidget* widget)
	{
		if (!widget) return;
		Cast<UCanvasPanel>(mainGameWidget->GetRootWidget())->AddChild(widget);

	}

};
