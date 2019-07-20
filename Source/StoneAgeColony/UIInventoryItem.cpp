// Fill out your copyright notice in the Description page of Project Settings.

#include "UIInventoryItem.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "StoneAgeColonyCharacter.h"
#include "UsableActor.h"
#include "Communicator.h"

UUIInventoryItem::UUIInventoryItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UUIInventoryItem::OnClick()
{
	UE_LOG(LogTemp, Warning, TEXT("Inventory Item Clicked, ID: %d"), ItemID);
	
}

void UUIInventoryItem::SetupInventoryItemCell()
{
	Super::SetupInventoryItemCell();

}

// Called from blueprint
void UUIInventoryItem::Use()
{
	//AStoneAgeColonyCharacter* PlayerCharacter = (AStoneAgeColonyCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	UE_LOG(LogTemp, Warning, TEXT("UUIInventoryItem::Use, ID: %d"), ItemID);

	if (ItemAmount > 0)
	{
		Communicator::GetInstance().UsableItemIDMap[ItemID]->OnUsedFromInventory(PlayerCharacter);
		PlayerCharacter->Inventory.Emplace(ItemID, PlayerCharacter->Inventory[ItemID] - 1);
		ItemAmount = PlayerCharacter->Inventory[ItemID];
		PlayerCharacter->UIPlayerInventory->Refresh();

	}

	UE_LOG(LogTemp, Warning, TEXT("UUIInventoryItem::Use, ItemAmount: %d"), ItemAmount);
}