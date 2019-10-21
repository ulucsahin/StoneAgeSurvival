// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory.h"
#include "HumanCharacter.h"
#include "StoneAgeColonyCharacter.h"
#include "UIBottomBar.h"
#include "UIPlayerInventory.h"


UInventory::UInventory()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UInventory::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UInventory::AddItem(int32 Item, int32 Amount)
{
	/* Item is ID of item that we want to add to inventory. */

	// if we have atleast one instance of Item in inventory
	if (!Items.Contains(Item))
	{
		Items.Add(Item, Amount);
	}
	else
	{
		Items.Emplace(Item, Items[Item] + Amount);
	}

	auto OwnerIsPlayer = Owner->IsA(AStoneAgeColonyCharacter::StaticClass());
	if (OwnerIsPlayer)
	{
		auto Player = Cast<AStoneAgeColonyCharacter>(Owner);
		if (Player)
		{
			if (Player->BottomBar)
			{
				Player->BottomBar->Refresh();
			}

			if (Player->UIPlayerInventory)
			{
				Player->UIPlayerInventory->Refresh();
			}
		}
		
	}
	
}

bool UInventory::Contains(int32 Item)
{
	return Items.Contains(Item);
}

TMap<int32, int32> UInventory::GetItems()
{
	return Items;
}

void UInventory::Emplace(int32 ItemToEmplace, int32 EmplaceWith)
{
	Items.Emplace(ItemToEmplace, EmplaceWith);
}

void UInventory::ConsumeItem(int32 ItemID, int32 Amount)
{
	if (Contains(ItemID))
	{
		Emplace(ItemID, Items[ItemID] - Amount);

		// Refresh bottom bar and inventory menu if consuming item from player's inventory
		AStoneAgeColonyCharacter* Player = Cast<AStoneAgeColonyCharacter>(Owner);
		if (Player)
		{
			if (Player->BottomBar)
			{
				Player->BottomBar->Refresh();
			}

			if (Player->UIPlayerInventory)
			{
				Player->UIPlayerInventory->Refresh();
			}

		}

	}
	
}