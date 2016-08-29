// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "GameFramework/Actor.h"
#include "OptionsMenu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReturnClicked);

UENUM(BlueprintType)
enum class SettingsLevel : uint8
{
	OFF,
	LOW,
	MEDIUM,
	HIGH,
	EPIC,
	
	NONE,
};

UENUM(BlueprintType)
enum class ButtonSettingsType : uint8
{
	PRESET,
	AA,
	POSTPROCESSING,
	SHADOWS,
	TEXTURES,
	EFFECTS,
	DETAILMODE,
	MATERIALQUALITY,
	FOLIAGE,
	CUSTOM
};

UENUM(BlueprintType)
enum class SliderSettingsType : uint8
{
	RESOLUTION,
	CUSTOM
};
class  UOptionsMenu;

UCLASS()
class STALKERGHOSTS_API UOptionsSetting : public UUserWidget
{
public:
	GENERATED_BODY()
		virtual void initialize(UOptionsMenu* parent) {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		FString name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UOptionsMenu* optionParent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UEditableTextBox* nameBox;

	
	virtual void updateVisuals() {}
};
UCLASS()
class STALKERGHOSTS_API UButtonSetting : public UOptionsSetting
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TArray<SettingsLevel> aviableSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		int32 settingsIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UButton* increaseButton;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UButton* decreaseButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UEditableTextBox* textBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		ButtonSettingsType type;

	UFUNCTION(BlueprintCallable, Category = "Event")
		void initialize(UOptionsMenu* parent) override;

	UFUNCTION(BlueprintCallable, Category = "Event")
		void increase();

	UFUNCTION(BlueprintCallable, Category = "Event")
		void decrease();
	UFUNCTION(BlueprintCallable, Category = "Event")
	SettingsLevel getCurrentSetting();
	UFUNCTION(BlueprintCallable, Category = "Event")
		void setCurrentSetting(SettingsLevel level);
	void updateVisuals() override;
	
};

UCLASS()
class STALKERGHOSTS_API UStringButtonSetting : public UOptionsSetting
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TArray<FString> displaySettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TArray<FString> underlyingSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		int32 settingsIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		int32 maximumSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UButton* increaseButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UButton* decreaseButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UEditableTextBox* textBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		FString itendifier;

	UFUNCTION(BlueprintCallable, Category = "Event")
		void initialize(UOptionsMenu* parent) override;

	UFUNCTION(BlueprintCallable, Category = "Event")
		void increase();

	UFUNCTION(BlueprintCallable, Category = "Event")
		void decrease();

	UFUNCTION(BlueprintCallable, Category = "Event")
		FString getCurrentSetting();

	UFUNCTION(BlueprintCallable, Category = "Event")
		FString getCurrentDisplay();

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	SliderSettingsType type;
	UFUNCTION(BlueprintCallable, Category = "Event")
		void valueChanged(float value);
	UFUNCTION(BlueprintCallable, Category = "Event")
		void textChanged(const FText& text, ETextCommit::Type commitType);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UEditableTextBox* textBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		USlider* slider;
	UFUNCTION(BlueprintCallable, Category = "Event")
		float getCurrentSetting();
	UFUNCTION(BlueprintCallable, Category = "Event")
		void initialize(UOptionsMenu* parent) override;
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
		TArray<UButtonSetting*> buttonWidgets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TArray<USliderSetting*> sliderWidgets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		TArray<UStringButtonSetting*> stringButtonWidgets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UButton* returnButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
		UWorld* world;
	UFUNCTION(BlueprintCallable, Category = "Event")
		void initialize();
	UFUNCTION(BlueprintCallable, Category = "Event")
		void settingsChanged(UOptionsSetting* sender);
	UFUNCTION(BlueprintCallable, Category = "Event")
		void closeButtonPressed();
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
		FOnReturnClicked OnClicked;
};
