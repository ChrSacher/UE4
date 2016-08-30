// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "GameFramework/Actor.h"
#include "OptionsMenu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReturnClicked);




UCLASS()
class STALKERGHOSTS_API UOptionsSetting : public UUserWidget
{
public:
	GENERATED_BODY()
		virtual void initialize(UOptionsMenu* newParent) {};
	void initializeExplanation();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UButton* explanationButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		FString consoleCommand;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UEditableTextBox* nameBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UOptionsMenu* optionParent;
	virtual void updateSettings() {}
	virtual void updateVisuals() {}
	virtual FString getCurrentExplanation() { return ""; }
	virtual void reinitialize() {};
	void onHovered();
	void onLeftHovered();
	UFUNCTION(BlueprintImplementableEvent)
		void settingsChanged();
	UFUNCTION(BlueprintImplementableEvent)
		void onStartup();
};
USTRUCT(Blueprintable)
struct FStringButtonSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = Weapon)
		FString displayedSetting;

	UPROPERTY(EditAnywhere, Category = Weapon)
		int32 underlyingSetting;

	UPROPERTY(EditAnywhere, Category = Weapon)
		FString optionExplanation;
};

UCLASS()
class STALKERGHOSTS_API UStringButtonSetting : public UOptionsSetting
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TArray<FStringButtonSettings> aviableSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		int32 settingsIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UButton* increaseButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UButton* decreaseButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UEditableTextBox* textBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		FString itendifier;

	UFUNCTION(BlueprintCallable, Category = "Event")
		void initialize(UOptionsMenu* newParent) override;

	UFUNCTION(BlueprintCallable, Category = "Event")
		void increase();

	UFUNCTION(BlueprintCallable, Category = "Event")
		void decrease();

	UFUNCTION(BlueprintCallable, Category = "Event")
		FStringButtonSettings getCurrentSetting();

	UFUNCTION(BlueprintCallable, Category = "Event")
		FString getCurrentDisplay();

	UFUNCTION(BlueprintCallable, Category = "Event")
	void setCurrentSetting(int32 level);
	UFUNCTION(BlueprintCallable, Category = "Event")
	void updateSettings() override;
	void reinitialize() ;
	FString getCurrentExplanation() override;
};

UCLASS()
class STALKERGHOSTS_API USliderSetting : public UOptionsSetting
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		float minimum = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		float maximum = 200;

	UPROPERTY(EditAnywhere, Category = Weapon)
		FString optionExplanation;
	UFUNCTION(BlueprintCallable, Category = "Event")
		void valueChanged(float value);
	UFUNCTION(BlueprintCallable, Category = "Event")
		void setValue(float value);
	UFUNCTION(BlueprintCallable, Category = "Event")
		void textChanged(const FText& text, ETextCommit::Type commitType);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UEditableTextBox* textBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		USlider* slider;
	UFUNCTION(BlueprintCallable, Category = "Event")
		float getCurrentSetting();
	UFUNCTION(BlueprintCallable, Category = "Event")
		void initialize(UOptionsMenu* newParent) override;
	void updateSettings() override;
	void reinitialize();
	FString getCurrentExplanation() override;
};

UCLASS()
class STALKERGHOSTS_API UAudioSliderSetting : public USliderSetting
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		USoundClass* soundClass;

};
/**
 * 
 */

UCLASS()
class STALKERGHOSTS_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()
public:

};

UCLASS()
class STALKERGHOSTS_API UOptionsMenu : public UUserWidget
{
	
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TArray<UOptionsSetting*> optionWidgets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UButton* returnButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UWorld* world;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UMultiLineEditableTextBox* box;
	UFUNCTION(BlueprintCallable, Category = "Event")
		void initialize();
	UFUNCTION(BlueprintCallable, Category = "Event")
		void settingsChanged(UOptionsSetting* sender);
	UFUNCTION(BlueprintCallable, Category = "Event")
		void closeButtonPressed();
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
		FOnReturnClicked OnClicked;

	UFUNCTION(BlueprintCallable, Category = "Event")
		void setExplainationText(FString explanationSetting);
};
