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
			return character->currentAttributes->health / character->currentAttributes->maxHealth;
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
			return character->currentAttributes->stamina / character->currentAttributes->maxStamina;
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
			return FString::SanitizeFloat(character->currentAttributes->health) + "/" + FString::SanitizeFloat(character->currentAttributes->maxHealth);
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
			return FString::SanitizeFloat(character->currentAttributes->stamina) + "/" + FString::SanitizeFloat(character->currentAttributes->maxStamina);
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
				return FString::FromInt(character->currentWeapon->weapon->currentAmmoCount) + "/" + FString::FromInt(character->currentWeapon->weapon->ammoCapacity);
			}
		}
	}
	return "ERROR";
}