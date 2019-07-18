// Fill out your copyright notice in the Description page of Project Settings.

#include "BottomBarItem.h"
#include "Communicator.h"
#include "UIBottomBar.h"

void UBottomBarItem::SetupBarItem(UUIBottomBar* BottomBar, int32 Index)
{
	Super::SetupInventoryItemCell();
	this->BottomBar = BottomBar;
	this->IndexInBottomBar = Index;
}

void UBottomBarItem::NotifyBottomBar()
{
	/* Notified bottom bar on changed made to this object so bottom bar can refresh */
	if (BottomBar)
	{
		BottomBar->GetNotification(this);
	}
	
}

void UBottomBarItem::UseBarItem()
{
	UE_LOG(LogTemp, Warning, TEXT("UBottomBarItem::UseBarItem"));
	if (ItemAmount > 0)
	{
		if (BottomBar->Player)
		{
			Communicator::GetInstance().UsableItemIDMap[ItemID]->OnUsed(BottomBar->Player);
			ItemAmount--;
			Update();
		}
		
	}
	
}

void UBottomBarItem::Update()
{
	if (BottomBar)
	{
		if (BottomBar->Player)
		{
			auto PlayerInventory = BottomBar->Player->Inventory;

			if (PlayerInventory.Contains(ItemID))
			{
				ItemAmount = PlayerInventory[ItemID];
				//NotifyBottomBar();
			}
			else 
			{
				ItemAmount = 0;
			}
			
		}
	}
	
}

