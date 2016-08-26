// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "StalkerGhostsCharacter.h"
#include "OptionsMenu.h"


FString SLtoString(SettingsLevel type)
{
	switch (type)
	{
		case SettingsLevel::OFF:
		{
			return "Off";
		}; break;
		case 	SettingsLevel::VERYLOW:
		{
			return "Very low";
		}; break;
		case 	SettingsLevel::LOW:
		{
			return "Low";
		}; break;
		case SettingsLevel::MEDIUM:
		{
			return "Medium";
		}; break;
		case 	SettingsLevel::HIGH:
		{
			return "High";
		}; break;
		case 	SettingsLevel::VERYHIGH:
		{
			return "Very High";
		}; break;
		case 	SettingsLevel::ULTRA:
		{
			return "Ultra";
		}; break;
		case 	SettingsLevel::EPIC:
		{
			return "Epic";
		}; break;
		case SettingsLevel::NONE:
		{
			return "None";
		}; break;
	}
	return "ERROR";
}
void UButtonSetting::initialize(UOptionsMenu* parent)
{
	if (aviableSettings.Num() > 0)
	{
		increaseButton->OnClicked.AddDynamic(this, &UButtonSetting::increase);
		decreaseButton->OnClicked.AddDynamic(this, &UButtonSetting::decrease);
		settingsIndex = FMath::Clamp<int32>(settingsIndex, 0, aviableSettings.Num() - 1);
	}
	nameBox->SetText(FText::FromString(name));
	textBox->SetText(FText::FromString(SLtoString(getCurrentSetting())));
	optionParent = parent;
	
}

void UButtonSetting::increase()
{
	if(aviableSettings.Num() == 0) return;
	settingsIndex = FMath::Clamp(settingsIndex, 0, aviableSettings.Num() -1);
	if (settingsIndex < aviableSettings.Num() - 1)
	{
		settingsIndex += 1;
	}
	settingsIndex = FMath::Clamp(settingsIndex, 0, aviableSettings.Num() - 1);
	if (settingsIndex == aviableSettings.Num() - 1)
	{
		increaseButton->bIsEnabled = false;
		decreaseButton->bIsEnabled = true;
	}
	if (settingsIndex == 0)
	{
		decreaseButton->bIsEnabled = false;
		increaseButton->bIsEnabled = true;
	}
	textBox->SetText(FText::FromString(SLtoString(getCurrentSetting())));
	optionParent->settingsChanged(this);
}

void UButtonSetting::decrease()
{
	if (aviableSettings.Num() == 0) return;
	
	if (settingsIndex < aviableSettings.Num())
	{
		settingsIndex -= 1;
	}
	settingsIndex = FMath::Clamp(settingsIndex, 0, aviableSettings.Num() - 1);
	if (settingsIndex == aviableSettings.Num() - 1)
	{
		increaseButton->bIsEnabled = false;
		decreaseButton->bIsEnabled = true;
	}
	if (settingsIndex == 0)
	{
		decreaseButton->bIsEnabled = false;
		increaseButton->bIsEnabled = true;
	}
	textBox->SetText(FText::FromString(SLtoString(getCurrentSetting())));
	optionParent->settingsChanged(this);
}
SettingsLevel UButtonSetting::getCurrentSetting()
{
	if (aviableSettings.Num() > 0)
	{
		settingsIndex = FMath::Clamp(settingsIndex, 0, aviableSettings.Num() - 1);
		return aviableSettings[settingsIndex];
	}
	return SettingsLevel::NONE;
	
}

float USliderSetting::getCurrentSetting()
{
	return slider->GetValue() * (maximum - minimum) + minimum;
}
void USliderSetting::initialize(UOptionsMenu* parent)
{
	optionParent = parent;
	nameBox->SetText(FText::FromString(name));
	slider->OnValueChanged.AddDynamic(this, &USliderSetting::valueChanged);
	textBox->OnTextCommitted.RemoveDynamic(this, &USliderSetting::textChanged);
	textBox->OnTextCommitted.AddDynamic(this, &USliderSetting::textChanged);
}
void USliderSetting::valueChanged(float value)
{
	float x = value;
	slider->SetValue(x);
	textBox->SetText(FText::FromString(FString::FromInt(getCurrentSetting())));
	optionParent->settingsChanged(this);
}
void USliderSetting::textChanged(const FText& text, ETextCommit::Type commitType)
{
	FText x = textBox->GetText();
	FString w = x.ToString();
	if (!w.IsNumeric())
	{
		textBox->SetText(FText::FromString(FString::FromInt(getCurrentSetting())));
		return;
	}
	float y = FCString::Atof(*w);
	y = FMath::Clamp(y, minimum, maximum );
	slider->SetValue(y / maximum );
	textBox->SetText(FText::FromString(FString::FromInt(y)));
	optionParent->settingsChanged(this);
}


void UOptionsMenu::initialize()
{
	for (int32 i = 0; i <buttonWidgets.Num(); i++)
	{
		if (buttonWidgets[i]) buttonWidgets[i]->initialize(this);
	}
	for (int32 i = 0; i < sliderWidgets.Num(); i++)
	{
		if (sliderWidgets[i]) sliderWidgets[i]->initialize(this);
	}
	returnButton->OnClicked.AddDynamic(this, &UOptionsMenu::closeButtonPressed);
}
void UOptionsMenu::settingsChanged(UOptionsSetting* sender)
{
	//do everything here
}

void UOptionsMenu::closeButtonPressed()
{
	OnClicked.Broadcast();
}