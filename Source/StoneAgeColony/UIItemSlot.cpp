// Fill out your copyright notice in the Description page of Project Settings.

#include "UIItemSlot.h"
#include "Communicator.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "StoneAgeColonyCharacter.h"
#include "ObjectFactory.h"
#include "UsableActor.h"
#include "Inventory.h"

UUIItemSlot::UUIItemSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UUIItemSlot::SetupInventoryItemCell()
{
	PlayerCharacter = (AStoneAgeColonyCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (PlayerCharacter)
	{
		// Set texture and amount variables
		//UE_LOG(LogTemp, Warning, TEXT("UUIItemSlot::SetupInventoryItemCell Item ID: %d"), ItemID);
		//InventoryTexture = Communicator::GetInstance().UsableItemIDMap[ItemID]->InventoryTexture;

		AObjectFactory* Factory = NewObject<AObjectFactory>();
		auto SlotObject =  Factory->CreateObjectBetter(ItemID);
		InventoryTexture = SlotObject->InventoryTexture;
		ItemName = Factory->GetObjectNameFromID(ItemID);

		if (PlayerCharacter->Inventory->Contains(ItemID))
		{
			ItemAmount = PlayerCharacter->Inventory->Items[ItemID];
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
