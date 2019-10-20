// Fill out your copyright notice in the Description page of Project Settings.

#include "BottomBarItem.h"
#include "Communicator.h"
#include "UIBottomBar.h"
#include "ObjectFactory.h"
#include "UsableActor.h"
#include "Blueprint/UserWidget.h"
#include "StoneAgeColonyCharacter.h"
#include "Inventory.h"

void UBottomBarItem::SetupBarItem(UUIBottomBar* BottomBar, int32 Index)
{
	Super::SetupInventoryItemCell();
	this->BottomBar = BottomBar;
	this->IndexInBottomBar = Index;
}

void UBottomBarItem::NotifyBottomBar()
{
	/* Notified bottom bar on changes made to this object so bottom bar can refresh */
	if (BottomBar)
	{
		BottomBar->GetNotification(this);
	}
	
}

void UBottomBarItem::UseBarItem()
{
	if (BottomBar->Player)
	{
		AObjectFactory* Factory = NewObject<AObjectFactory>();
		auto BarItem = Factory->CreateObjectBetter(ItemID);
		BarItem->OnUsedFromInventory(BottomBar->Player);
		//Communicator::GetInstance().UsableItemIDMap[ItemID]->OnUsedFromInventory(BottomBar->Player);
		ItemAmount = BottomBar->Player->Inventory->Items[ItemID];
	}
	
}

void UBottomBarItem::Refresh()
{
	if (BottomBar)
	{
		if (BottomBar->Player)
		{
			auto PlayerInventory = BottomBar->Player->Inventory;

			if (PlayerInventory->Contains(ItemID))
			{
				ItemAmount = PlayerInventory->Items[ItemID];
				//NotifyBottomBar();
			}
			else 
			{
				ItemAmount = 0;
			}
			
		}
	}
	
}

