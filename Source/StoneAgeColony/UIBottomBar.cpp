// Fill out your copyright notice in the Description page of Project Settings.

#include "UIBottomBar.h"
#include "UObject/ConstructorHelpers.h"
//#include "Runtime/UMG/Public/Blueprint/WidgetTree.h"
//#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "BottomBarItem.h"
#include "StoneAgeColonyCharacter.h"
#include "Runtime/UMG/Public/Components/WrapBox.h"
#include "Communicator.h"

UUIBottomBar::UUIBottomBar(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//static ConstructorHelpers::FClassFinder<UUIInventoryItem> BPClass(TEXT("'/Game/Uluc/HUD/Inventory/InventoryItem.InventoryItem_C'"));
	static ConstructorHelpers::FClassFinder<UBottomBarItem> BPClass(TEXT("'/Game/Uluc/HUD/BottomItemBar/BottomBarItem.BottomBarItem_C'"));
	InvItemClass = BPClass.Class;
	
	for (int i = 0; i < 8; i++)
	{
		BarItemIDs.Add(0);
	}

	SelectedSlot = 0;
	
}

// Called in blueprint
void UUIBottomBar::RegisterToPlayer(AStoneAgeColonyCharacter* Player)
{
	this->Player = Player;
	Player->BottomBar = this;

	//UE_LOG(LogTemp, Warning, TEXT("UUIBottomBar::RegisterToPlayerxD"));
}

// Called in blueprint
void UUIBottomBar::InitializeBottomBarItems()
{
	/* Initialize bar items */
	int32 Index = 0;
	for (auto ID : BarItemIDs)//Player->GetInventory())
	{
		//item.key = ItemID, item.value = amount of that item
		auto BarItem = CreateWidget<UBottomBarItem>((APlayerController*)Player->GetController(), InvItemClass);
		BarItem->ItemID = ID; // item.Key;
		BarItem->SetupBarItem(this, Index);	

		// WrapBox assigned from blueprint
		WrapBox->AddChild(BarItem);
		BarItems.Add(BarItem);
	
		Index++;
	}

	SelectSlot(0);
	
}

void UUIBottomBar::RestoreBottomBarItemsFromSave()
{
	int32 Index = 0;
	for (auto ID : BarItemIDs)//Player->GetInventory())
	{
		BarItems[Index]->ItemID = ID; // item.Key;
		BarItems[Index]->SetupBarItem(this, Index);

		Index++;
	}

	SelectSlot(0);
}

void UUIBottomBar::AddItems()
{
	/* Add items in BarItems array to bottom bar */

	for (auto Item : BarItems)
	{
		WrapBox->AddChild(Item);
	}
	
}

void UUIBottomBar::SetItemAtIndex(UBottomBarItem* Item)
{
	//UE_LOG(LogTemp, Warning, TEXT("UUIBottomBar::SetItemAtIndex Index: %d"), Index);

	auto BarItem = CreateWidget<UBottomBarItem>((APlayerController*)Player->GetController(), InvItemClass);
	BarItem->ItemID = Item->ItemID;
	BarItem->SetupBarItem(this, Item->IndexInBottomBar);
	BarItems[Item->IndexInBottomBar] = BarItem;
	Refresh();
}

void UUIBottomBar::Refresh()
{
	/* Also registers ItemIDs of baritems to save file */
	//UE_LOG(LogTemp, Warning, TEXT("UUIBottomBar::Refresh"));
	WrapBox->ClearChildren();
	int i = 0;
	for (auto Item : BarItems)
	{
		Item->Update();
		BarItemIDs[i] = Item->ItemID;
		i++;
	}
	AddItems();

	// Register current BarItemIDs to save file
	Communicator::GetInstance().BottomBarItems = BarItemIDs;
}

void UUIBottomBar::SelectSlot(int32 Index)
{
	// Set previously selected item to normal scale and alpha
	BarItems[SelectedSlot]->SetRenderScale(FVector2D(1.f, 1.f));

	// Set newly selected item
	SelectedSlot = Index;
	BarItems[SelectedSlot]->SetRenderScale(FVector2D(1.2f, 1.2f));
	
}

void UUIBottomBar::SelectNextSlot()
{
	int32 NewSlot = (SelectedSlot + 1) % 8;
	SelectSlot(NewSlot);
}

void UUIBottomBar::SelectPreviousSlot()
{
	int32 NewSlot = (SelectedSlot - 1) % 8;
	if (NewSlot < 0) NewSlot += 8;

	SelectSlot(NewSlot);
}

void UUIBottomBar::GetNotification(UBottomBarItem* Item)
{
	/* Gets notification from "Item", means there is a change about "Item" */
	SetItemAtIndex(Item);

	// Re-select
	SelectSlot(SelectedSlot); 
}

bool UUIBottomBar::IsItemInBar(int32 ItemID)
{
	bool result = false;
	for (auto Item : BarItems)
	{
		if (Item->ItemID == ItemID)
		{
			result = true;
			break;
		}
	}
		
	return result;
}