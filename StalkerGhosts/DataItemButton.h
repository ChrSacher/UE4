// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Button.h"
#include "DataItemButton.generated.h"

class UItemBase;
/**
 * 
 */
UCLASS()
class STALKERGHOSTS_API UDataItemButton : public UButton
{
	GENERATED_BODY()
	DECLARE_DELEGATE_OneParam(UDataItemButtonDelegate, UDataItemButton*)
	//can be made better but works for now
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = User)
		UItemBase* UserPointer;

	UDataItemButtonDelegate click;
	UDataItemButtonDelegate hover;
	UDataItemButtonDelegate unhover;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = User)
		uint8 UserNumber;
	//this is litteraly going against Design decision but it works
	bool CategoryIdentifier;
	UFUNCTION(BlueprintCallable, Category = "Event")
	void hasBeenClicked()
	{
		
		click.ExecuteIfBound(this);
	}
	UFUNCTION(BlueprintCallable, Category = "Event")
	void hasBeenHovered()
	{
		
		hover.ExecuteIfBound(this);

	}
	UFUNCTION(BlueprintCallable, Category = "Event")
	void hasBeenUnHovered()
	{
		unhover.ExecuteIfBound(this);
		
	}

};
