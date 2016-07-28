// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "MainHudWidget.h"
#include "StalkerGhostsCharacter.h"


float UMainHudWidget::getHealthPercentageFromCharacter()
{
	if (character)
	{
		if (character->currentAttributes)
		{
			return character->currentAttributes->getAttrib(AttributeType::HEALTH)->getFinal()/ character->currentAttributes->getAttrib(AttributeType::MAXHEALTH)->getFinal();
		}
	}
	return 0;
}

float UMainHudWidget::getStaminaPercentageFromCharacter()
{
	if (character)
	{
		if (character->currentAttributes)
		{
			return character->currentAttributes->getAttrib(AttributeType::STAMINA)->getFinal() / character->currentAttributes->getAttrib(AttributeType::MAXSTAMINA)->getFinal();
		}
	}
	return 0;
}

FString UMainHudWidget::getHealthText()
{
	if (character)
	{
		if (character->currentAttributes)
		{
			return FString::SanitizeFloat(character->currentAttributes->getAttrib(AttributeType::HEALTH)->getFinal()) + "/" + FString::SanitizeFloat(character->currentAttributes->getAttrib(AttributeType::MAXHEALTH)->getFinal());
		}
	}
	return "ERROR";
}

FString UMainHudWidget::getStaminaText()
{
	if (character)
	{
		if (character->currentAttributes)
		{
			return FString::SanitizeFloat(character->currentAttributes->getAttrib(AttributeType::STAMINA)->getFinal()) + "/" + FString::SanitizeFloat(character->currentAttributes->getAttrib(AttributeType::MAXSTAMINA)->getFinal());
		}
	}
	return "ERROR";
}



FString UMainHudWidget::getAmmoText()
{
	if (character)
	{
		if (character->currentWeapon)
		{
			if (character->currentWeapon->weapon)
			{
				return FString::FromInt(character->currentWeapon->getAmmoCount()) + "/" + FString::FromInt(character->currentWeapon->weapon->ammoCapacity);
			}
		}
	}
	return "ERROR";
}