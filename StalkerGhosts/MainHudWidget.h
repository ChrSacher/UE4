// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "MainHudWidget.generated.h"

class AStalkerGhostsCharacter;
/**
 * 
 */
UCLASS()
class STALKERGHOSTS_API UMainHudWidget : public UUserWidget
{
	GENERATED_BODY()

		

public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
			AStalkerGhostsCharacter* character;

		
		UFUNCTION(BlueprintCallable, Category = "Event")
			float getHealthPercentageFromCharacter();

		UFUNCTION(BlueprintCallable, Category = "Event")
			float getStaminaPercentageFromCharacter();

		UFUNCTION(BlueprintCallable, Category = "Event")
			FString getHealthText();

		UFUNCTION(BlueprintCallable, Category = "Event")
			FString getStaminaText();

		UFUNCTION(BlueprintCallable, Category = "Event")
			FString getAmmoText();
};
