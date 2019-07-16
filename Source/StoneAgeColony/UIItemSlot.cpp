// Fill out your copyright notice in the Description page of Project Settings.

#include "UIItemSlot.h"
#include "StoneAgeColonyCharacter.h"
#include "Communicator.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

UUIItemSlot::UUIItemSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UUIItemSlot::SetupInventoryItemCell()
{
	AStoneAgeColonyCharacter* PlayerCharacter = (AStoneAgeColonyCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (PlayerCharacter)
	{
		// Set texture and amount variables
		InventoryTexture = Communicator::GetInstance().UsableItemIDMap[ItemID]->InventoryTexture;

		if (PlayerCharacter->Inventory.Contains(ItemID))
		{
			ItemAmount = PlayerCharacter->Inventory[ItemID];
		}
		else
		{
			ItemAmount = 0;
		}
		
	}
}

void UUIItemSlot::OnClick()
{
	// implemented in child classes
}
