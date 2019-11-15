// Fill out your copyright notice in the Description page of Project Settings.

#include "UIInventoryItem.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
//#include "StoneAgeColonyCharacter.h"
#include "UsableActor.h"
#include "Communicator.h"
//#include "PickupManager.h"
//#include "Runtime/Engine/Classes/Engine/StreamableManager.h"
//#include "Runtime/Engine/Classes/Engine/AssetManager.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "ObjectFactory.h"
#include "StoneAgeColonyCharacter.h"
#include "UIPlayerInventory.h"
#include "Inventory.h"
#include "UIBottomBar.h"

UUIInventoryItem::UUIInventoryItem(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UUIInventoryItem::OnClick()
{
	//UE_LOG(LogTemp, Warning, TEXT("Inventory Item Clicked, ID: %d"), ItemID);
	
}

void UUIInventoryItem::SetupInventoryItemCell()
{
	Super::SetupInventoryItemCell();

}

// Called from blueprint
void UUIInventoryItem::Use()
{
	if (Owner)
	{
		if (ItemAmount > 0)
		{
			AObjectFactory* Factory = NewObject<AObjectFactory>();
			auto ItemToUse = Factory->CreateObjectBetter(ItemID);
			ItemToUse->OnUsedFromInventory(Owner); // this may create bugs or may not work as its supposed to be
			Owner->Inventory->Emplace(ItemID, Owner->Inventory->GetItems()[ItemID] - 1);
			ItemAmount = Owner->Inventory->GetItems()[ItemID];
			auto Player = Cast<AStoneAgeColonyCharacter>(Owner);
			Owner->UIPlayerInventory->Refresh();

			if (Player)	Player->BottomBar->Refresh();
			
		}

	}

}

void UUIInventoryItem::PlaceItem()
{
	/* Spawns item in slot into world */

	if (ItemAmount <= 0)
	{
		Owner->UIPlayerInventory->Refresh();
		return;
	}
	auto Player = Cast<AStoneAgeColonyCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	// Calculate location to spawn (in front of player)
	auto PlayerForwardVector = Player->GetActorForwardVector();
	auto SpawnLocation = Player->GetActorLocation();
	SpawnLocation.X += PlayerForwardVector.X * 150.f;
	SpawnLocation.Y += PlayerForwardVector.Y * 150.f;

	AObjectFactory* Factory = NewObject<AObjectFactory>();
	auto ObjectToPlace = Factory->CreateObjectBetter(ItemID);
	
	auto ClassToSpawn = ObjectToPlace->GetClass(); //Communicator::GetInstance().UsableItemIDMap[ItemID]->GetClass();
	
	const FString ContextString(TEXT("Spawned Type Context"));

	auto SpawnedItem = Communicator::GetInstance().World->SpawnActor<AUsableActor>(ClassToSpawn, SpawnLocation, FRotator::ZeroRotator);
	auto ObjectName = Factory->GetObjectNameFromID(ItemID);
	SpawnedItem->SetupType(ObjectName);
	SpawnedItem->SetMeshToDefault();

	// Adjust player inventory since item is used
	Owner->Inventory->Emplace(ItemID, Owner->Inventory->Items[ItemID] - 1);
	ItemAmount = Owner->Inventory->Items[ItemID];
	Owner->UIPlayerInventory->Refresh();

	
	if (Player)
	{
		Player->CloseAllMenus();
		Player->PlayerStates = EPlayerStates::VE_Pickup;
		Player->InteractPickup(SpawnedItem);
	}
}