// Fill out your copyright notice in the Description page of Project Settings.

#include "StalkerGhosts.h"
#include "MainInventoryWidget.h"



void UMainInventoryWidget::setEquippedWidget(SlotInformation sslot, UEquippedItemWidget* widget)
{
	widgets[(uint8)sslot] = widget;
}
UEquippedItemWidget* UMainInventoryWidget::getEquippedWidget(SlotInformation sslot)
{
	return widgets[(uint8)sslot];
}

void UMainInventoryWidget::setup()
{
	for (int i = 0; i < (uint8)SlotInformation::NUM; i++)
	{
		widgets.Add(NULL);
	}
}