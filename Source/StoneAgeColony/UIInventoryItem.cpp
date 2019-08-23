// Fill out your copyright notice in the Description page of Project Settings.

#include "UIInventoryItem.h"
//#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
//#include "StoneAgeColonyCharacter.h"
#include "UsableActor.h"
#include "Communicator.h"
//#include "PickupManager.h"
//#include "Runtime/Engine/Classes/Engine/StreamableManager.h"
//#include "Runtime/Engine/Classes/Engine/AssetManager.h"
#include "ObjectFactory.h"

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
		AObjectFactory* Factory = NewObject<AObjectFactory>();
		auto ItemToUse = Factory->CreateObjectBetter(ItemID);
		ItemToUse->OnUsedFromInventory(PlayerCharacter);

		//Communicator::GetInstance().UsableItemIDMap[ItemID]->OnUsedFromInventory(PlayerCharacter);
		PlayerCharacter->Inventory.Emplace(ItemID, PlayerCharacter->Inventory[ItemID] - 1);
		ItemAmount = PlayerCharacter->Inventory[ItemID];
		PlayerCharacter->UIPlayerInventory->Refresh();
	}

	UE_LOG(LogTemp, Warning, TEXT("UUIInventoryItem::Use, ItemAmount: %d"), ItemAmount);
}

void UUIInventoryItem::PlaceItem()
{
	/* Spawns item in slot into world */

	if (ItemAmount <= 0)
	{
		PlayerCharacter->UIPlayerInventory->Refresh();
		return;
	}

	// Calculate location to spawn (in front of player)
	auto PlayerForwardVector = PlayerCharacter->GetActorForwardVector();
	auto SpawnLocation = PlayerCharacter->GetActorLocation();
	SpawnLocation.X += PlayerForwardVector.X * 150.f;
	SpawnLocation.Y += PlayerForwardVector.Y * 150.f;

	AObjectFactory* Factory = NewObject<AObjectFactory>();
	auto ObjectToPlace = Factory->CreateObjectBetter(ItemID);
	
	auto ClassToSpawn = ObjectToPlace->GetClass(); //Communicator::GetInstance().UsableItemIDMap[ItemID]->GetClass();
	
	const FString ContextString(TEXT("Spawned Type Context"));
	auto IDAsString = FString::FromInt(ItemID);
	auto IDAsFName = FName(*IDAsString);
	auto ObjectNameData = Communicator::GetInstance().ObjectNameDataTable->FindRow<FObjectNameData>(IDAsFName, ContextString, true);

	auto SpawnedItem = Communicator::GetInstance().World->SpawnActor<AUsableActor>(ClassToSpawn, SpawnLocation, FRotator::ZeroRotator);
	SpawnedItem->SetupType(ObjectNameData->Name);
	SpawnedItem->SetMeshToDefault();

	// Adjust player inventory since item is used
	PlayerCharacter->Inventory.Emplace(ItemID, PlayerCharacter->Inventory[ItemID] - 1);
	ItemAmount = PlayerCharacter->Inventory[ItemID];
	PlayerCharacter->UIPlayerInventory->Refresh();

	PlayerCharacter->InteractPickup(SpawnedItem);

}