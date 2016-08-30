// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "StalkerGhostsCharacter.h"
#include "OptionsMenu.h"

void UOptionsSetting::initializeExplanation()
{
	if (!explanationButton) return;
	explanationButton->OnHovered.AddDynamic(this, &UOptionsSetting::onHovered);
	explanationButton->OnUnhovered.AddDynamic(this, &UOptionsSetting::onLeftHovered);
}
void UOptionsSetting::onHovered()
{
	
		optionParent->setExplainationText(getCurrentExplanation());
}
void UOptionsSetting::onLeftHovered()
{
	FString x = "";
	optionParent->setExplainationText(x);
}



void UStringButtonSetting::initialize(UOptionsMenu* parent)
{
	increaseButton->OnClicked.AddDynamic(this, &UStringButtonSetting::increase);
	decreaseButton->OnClicked.AddDynamic(this, &UStringButtonSetting::decrease);
	settingsIndex = FMath::Clamp(settingsIndex, 0, aviableSettings.Num() - 1);
	nameBox->SetText(FText::FromString(name));
	reinitialize();
	updateVisuals();
	optionParent = parent;
	onStartup();
}

void UStringButtonSetting::reinitialize()
{
	auto CVar = IConsoleManager::Get().FindConsoleVariable(*consoleCommand);
	int32 Value = CVar->GetInt();
	setCurrentSetting(Value);
}
void UStringButtonSetting::increase()
{
	if (aviableSettings.Num() == 0) return;
	int32 lastSetting = settingsIndex;
	settingsIndex += 1;
	settingsIndex = FMath::Clamp(settingsIndex, 0, aviableSettings.Num() - 1);
	textBox->SetText(FText::FromString((getCurrentDisplay())));
	if (lastSetting != settingsIndex) updateSettings();
}


void UStringButtonSetting::decrease()
{
	if (aviableSettings.Num() == 0) return;
	int32 lastSetting = settingsIndex;
	settingsIndex -= 1;
	settingsIndex = FMath::Clamp(settingsIndex, 0, aviableSettings.Num() - 1);
	textBox->SetText(FText::FromString(getCurrentDisplay()));
	if (lastSetting != settingsIndex)updateSettings();
}
FStringButtonSettings  UStringButtonSetting::getCurrentSetting()
{
	if (aviableSettings.Num() > 0)
	{
		settingsIndex = FMath::Clamp(settingsIndex, 0, aviableSettings.Num() - 1);

		return aviableSettings[settingsIndex];
	}
	return FStringButtonSettings();
}

FString  UStringButtonSetting::getCurrentDisplay()
{
	if (aviableSettings.Num() > 0)
	{
		settingsIndex = FMath::Clamp(settingsIndex, 0, aviableSettings.Num() - 1);

		return aviableSettings[settingsIndex].displayedSetting;
	}
	return "ERROR";
	
}
void UStringButtonSetting::setCurrentSetting(int32 level)
{
	int32 lastSetting = settingsIndex;
	for (int32 i = 0; i < aviableSettings.Num(); i++)
	{
		if (aviableSettings[i].underlyingSetting == level)
		{
			settingsIndex = i;
			if (lastSetting != settingsIndex) updateSettings();
			updateVisuals();
			return;
		}
	}
	settingsIndex = FMath::Clamp(settingsIndex, 0, aviableSettings.Num() - 1);
	if (lastSetting != settingsIndex) updateSettings();
	updateVisuals();
}
void  UStringButtonSetting::updateSettings()
{
	optionParent->settingsChanged(this);
	settingsChanged();
}
FString UStringButtonSetting::getCurrentExplanation()
{
	return getCurrentSetting().optionExplanation;
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
	textBox->OnTextCommitted.AddDynamic(this, &USliderSetting::textChanged);
	reinitialize();
	onStartup();
}
void USliderSetting::valueChanged(float value)
{
	float x = value;
	slider->SetValue(x);
	textBox->SetText(FText::FromString(FString::FromInt(getCurrentSetting())));
	updateSettings();
}

void USliderSetting::setValue(float value)
{
	value = FMath::Clamp(value, minimum, maximum);
	slider->SetValue(value / maximum);
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
	slider->SetValue((y - minimum) / (maximum - minimum));
	textBox->SetText(FText::FromString(FString::FromInt(y)));
	updateSettings();
	
}
void USliderSetting::reinitialize()
{
	auto CVar = IConsoleManager::Get().FindConsoleVariable(*consoleCommand);
	int32 Value = CVar->GetInt();
	setValue(Value);
}

void  USliderSetting::updateSettings()
{
	optionParent->settingsChanged(this);
	settingsChanged();
}

FString  USliderSetting::getCurrentExplanation()
{
	return optionExplanation;
}
void UOptionsMenu::initialize()
{

	for (int32 i = 0; i < optionWidgets.Num(); i++)
	{
		if (optionWidgets[i])optionWidgets[i]->initialize(this);
	}
	
	returnButton->OnClicked.AddDynamic(this, &UOptionsMenu::closeButtonPressed);
}


void UOptionsMenu::settingsChanged(UOptionsSetting* sender)
{
	if (!sender) return;
	UAudioSliderSetting* audioSender = Cast<UAudioSliderSetting>(sender);
	if (audioSender)
	{
		if(audioSender->soundClass) audioSender->soundClass->Properties.Volume = audioSender->getCurrentSetting();
	}
	else
	{
		USliderSetting* sliderSender = Cast<USliderSetting>(sender);
		if (sliderSender)
		{
			if (world)
			{
				FString Final = sliderSender->consoleCommand + " " + FString::FromInt(sliderSender->getCurrentSetting());

				world->Exec(GetWorld(), *Final);
			}
		}
	}
	UStringButtonSetting* stringSender = Cast<UStringButtonSetting>(sender);
	if(stringSender)
	{
		
		if (world)
		{
			FString Final = stringSender->consoleCommand + " " + stringSender->getCurrentSetting().underlyingSetting;

			world->Exec(GetWorld(), *Final);
		}
	}
	
	GEngine->GameUserSettings->ApplySettings(true);
	for (int32 i = 0; i < optionWidgets.Num(); i++)
	{
		if (optionWidgets[i])optionWidgets[i]->reinitialize();
	}
}

void UOptionsMenu::closeButtonPressed()
{
	OnClicked.Broadcast();
}
void UOptionsMenu::setExplainationText(FString explanationSetting)
{
	if (box) box->SetText(FText::FromString(explanationSetting));
}