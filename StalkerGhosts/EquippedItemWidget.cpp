// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "EquippedItemWidget.h"




UTexture2D* UEquippedItemWidget::getTexture()
{
	if (ItemButton)
	{
		if (ItemButton->UserPointer)
		{
			if (ItemButton->UserPointer->picture)
			{
				return ItemButton->UserPointer->picture;
			}
		}
	}
	return NULL;
}