// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "DataItemButton.h"
#include "ItemBase.h"
#include "ItemWidget.h"



UTexture2D*  UItemWidget::getTexture()
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


int32  UItemWidget::getAmmount()
{
	if (ItemButton)
	{
		if (ItemButton->UserPointer)
		{
			if (ItemButton->UserPointer->picture)
			{
				return ItemButton->UserPointer->ammount;
			}
		}
	}
	return 0;
}


FString UItemWidget::getName()
{
	if (ItemButton)
	{
		if (ItemButton->UserPointer)
		{
			if (ItemButton->UserPointer->picture)
			{
				return ItemButton->UserPointer->name;
			}
		}
	}
	return "ERROR";
}

