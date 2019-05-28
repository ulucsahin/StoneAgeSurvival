// Fill out your copyright notice in the Description page of Project Settings.

#include "UIInventoryItem.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "StoneAgeColonyCharacter.h"
#include "UsableActor.h"
#include "Communicator.h"

UUIInventoryItem::UUIInventoryItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
	
}

void UUIInventoryItem::SetupInventoryItemCell(const int &index)
{
	// Initialize CDO properties here.
	AStoneAgeColonyCharacter* PlayerCharacter = (AStoneAgeColonyCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (PlayerCharacter && index != NULL)
	{
		int CurrentItemId = 0;
		IndexInInventory = index;
		UE_LOG(LogTemp, Warning, TEXT("IndexInInventory: %d"), IndexInInventory);

		// Take item id in the index we want to reach
		if (IndexInInventory < PlayerCharacter->Inventory.Num()) 
		{
			CurrentItemId = PlayerCharacter->Inventory[IndexInInventory];
			UE_LOG(LogTemp, Warning, TEXT("CurrentItemId ID: %d"), CurrentItemId);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("IndexInInventory EXCEEDED INVENTORY BOUNDS !! "));
		}

		// Get ID of that item (this is redundant since we already store items by ID, this is only for clarity purposes)
		ItemID = Communicator::GetInstance().UsableItemIDMap[CurrentItemId]->GetID();
		UE_LOG(LogTemp, Warning, TEXT("ItemID: %d"), ItemID);
	}
}
