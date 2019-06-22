// Fill out your copyright notice in the Description page of Project Settings.

#include "UIInventoryItem.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "StoneAgeColonyCharacter.h"
#include "UsableActor.h"
#include "Communicator.h"

UUIInventoryItem::UUIInventoryItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
	
}


void UUIInventoryItem::SetupInventoryItemCell()
{
	AStoneAgeColonyCharacter* PlayerCharacter = (AStoneAgeColonyCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (PlayerCharacter)
	{
		// Set texture and amount variables
		InventoryTexture = Communicator::GetInstance().UsableItemIDMap[ItemID]->InventoryTexture;
		ItemAmount = PlayerCharacter->Inventory[ItemID];
	}

}