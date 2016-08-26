// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "StalkerGhostsCharacter.h"
#include "OptionsMenu.h"


FString SLtoString(SettingsLevel type)
{
	switch (type)
	{
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
		case 	SettingsLevel::EPIC:
		{
			return "Epic";
		}; break;
		case 	SettingsLevel::OFF:
		{
			return "Off";
		}; break;
		case 	SettingsLevel::NONE:
		{
			return "NONE";
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
	int32 lastSetting = settingsIndex;
	settingsIndex += 1;
	updateVisuals();
	if(lastSetting != settingsIndex)optionParent->settingsChanged(this);
}

void UButtonSetting::decrease()
{
	if (aviableSettings.Num() == 0) return;
	int32 lastSetting = settingsIndex;
	settingsIndex -= 1;
	updateVisuals();
	if (lastSetting != settingsIndex) optionParent->settingsChanged(this);
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
void  UButtonSetting::setCurrentSetting(SettingsLevel level)
{
	int32 x = aviableSettings.Find(level);
	if (x != INDEX_NONE)
	{
		settingsIndex = x;
	}
	else
	{
		settingsIndex = aviableSettings.Num() - 1;
	}
	updateVisuals();
}

void UButtonSetting::updateVisuals()
{
	settingsIndex = FMath::Clamp(settingsIndex, 0, aviableSettings.Num() - 1);
	textBox->SetText(FText::FromString(SLtoString(getCurrentSetting())));
}




void UStringButtonSetting::initialize(UOptionsMenu* parent)
{
	maximumSettings = FMath::Max(displaySettings.Num(), underlyingSettings.Num());
	if (displaySettings.Num() > 0 && underlyingSettings.Num() > 0)
	{
		increaseButton->OnClicked.AddDynamic(this, &UStringButtonSetting::increase);
		decreaseButton->OnClicked.AddDynamic(this, &UStringButtonSetting::decrease);
		
	}
	settingsIndex = FMath::Clamp(settingsIndex, 0, maximumSettings - 1);
	nameBox->SetText(FText::FromString(name));
	textBox->SetText(FText::FromString(getCurrentDisplay()));
	optionParent = parent;

}


void UStringButtonSetting::increase()
{
	if (maximumSettings == 0) return;
	int32 lastSetting = settingsIndex;
	settingsIndex += 1;
	settingsIndex = FMath::Clamp(settingsIndex, 0, maximumSettings - 1);
	textBox->SetText(FText::FromString((getCurrentDisplay())));
	if (lastSetting != settingsIndex)optionParent->settingsChanged(this);
}


void UStringButtonSetting::decrease()
{
	if (maximumSettings == 0) return;
	int32 lastSetting = settingsIndex;
	settingsIndex -= 1;
	settingsIndex = FMath::Clamp(settingsIndex, 0, maximumSettings - 1);
	textBox->SetText(FText::FromString(getCurrentDisplay()));
	if (lastSetting != settingsIndex)optionParent->settingsChanged(this);
}
FString  UStringButtonSetting::getCurrentSetting()
{
	if (maximumSettings > 0)
	{
		settingsIndex = FMath::Clamp(settingsIndex, 0, maximumSettings - 1);

		return underlyingSettings[settingsIndex];
	}
	return "ERROR";
}

FString  UStringButtonSetting::getCurrentDisplay()
{
	if (maximumSettings > 0)
	{
		settingsIndex = FMath::Clamp(settingsIndex, 0, maximumSettings - 1);

		return displaySettings[settingsIndex];
	}
	return "ERROR";
	
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
	for (int32 i = 0; i < stringButtonWidgets.Num(); i++)
	{
		if (stringButtonWidgets[i]) stringButtonWidgets[i]->initialize(this);
	}
	
	returnButton->OnClicked.AddDynamic(this, &UOptionsMenu::closeButtonPressed);
}
void UOptionsMenu::settingsChanged(UOptionsSetting* sender)
{
	if (!sender) return;
	UButtonSetting* buttonSender = Cast<UButtonSetting>(sender);
	switch(buttonSender->type)
	{

		case ButtonSettingsType::PRESET:
		{
			for (int32 i = 0; i < buttonWidgets.Num(); i++)
			{
				buttonWidgets[i]->setCurrentSetting(buttonSender->getCurrentSetting());
			};
			GEngine->GameUserSettings->SetOverallScalabilityLevel(int32(buttonSender->getCurrentSetting()));
			
		}break;
		case ButtonSettingsType::AA:
		{
			GEngine->GameUserSettings->SetAntiAliasingQuality(int32(buttonSender->getCurrentSetting()));
		}break;
		case ButtonSettingsType::POSTPROCESSING:
		{
			GEngine->GameUserSettings->SetPostProcessingQuality(int32(buttonSender->getCurrentSetting()));
		}break;
		case ButtonSettingsType::SHADOWS:
		{
			GEngine->GameUserSettings->SetShadowQuality(int32(buttonSender->getCurrentSetting()));
		}break;
		case ButtonSettingsType::TEXTURES:
		{
			GEngine->GameUserSettings->SetTextureQuality(int32(buttonSender->getCurrentSetting()) - 1); //- 1 for switching scales
		}break;
		case ButtonSettingsType::EFFECTS:
		{
			GEngine->GameUserSettings->SetVisualEffectQuality(int32(buttonSender->getCurrentSetting()) - 1);//- 1 for switching scales
		}break;
		case ButtonSettingsType::DETAILMODE:
		{
			if (world)
			{
				FString Final = "r.DetailMode " + FString::FromInt(int32(buttonSender->getCurrentSetting()) - 1);

				world->Exec(GetWorld(), *Final);
			}
		}break;
		case ButtonSettingsType::MATERIALQUALITY:
		{
			if (world)
			{
				FString Final = "r.MaterialQualityLevel " + FString::FromInt(int32(buttonSender->getCurrentSetting()) - 1);

				world->Exec(GetWorld(), *Final);
			}
		}break;
		case ButtonSettingsType::FOLIAGE:
		{
			GEngine->GameUserSettings->SetFoliageQuality(int32(buttonSender->getCurrentSetting()) - 1);//- 1 for switching scales
		}break;
		case ButtonSettingsType::CUSTOM:
		{
			//duno
		}break;
	};
	GEngine->GameUserSettings->ApplySettings(true);
}

void UOptionsMenu::closeButtonPressed()
{
	OnClicked.Broadcast();
}