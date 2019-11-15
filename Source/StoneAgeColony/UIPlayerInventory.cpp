// Fill out your copyright notice in the Description page of Project Settings.

#include "UIPlayerInventory.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/UMG/Public/Components/WrapBox.h"
#include "UIInventoryItem.h"
#include "Inventory.h"
#include "StoneAgeColonyCharacter.h"
#include "HumanCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

UUIPlayerInventory::UUIPlayerInventory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUIInventoryItem> BPClass(TEXT("'/Game/Uluc/HUD/Inventory/InventoryItem.InventoryItem_C'"));
	InvItemClass = BPClass.Class;
}

void UUIPlayerInventory::InitialSetup(AHumanCharacter* Owner)
{
	SetOwner(Owner);
	RegisterToPlayer();
	AddItems();

	auto PlayerController = (APlayerController*)Player->GetController();
	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeGameAndUI());
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
	}
}

void UUIPlayerInventory::SetOwner(AHumanCharacter* Owner)
{
	this->Owner = Owner;
}

void UUIPlayerInventory::RegisterToPlayer()
{
	Player = (AStoneAgeColonyCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (Owner == Player)
	{
		Player->UIPlayerInventory = this;
	}
	
}

void UUIPlayerInventory::AddItems()
{
	if (Player)
	{
		if (Owner)
		{
			auto Inventory = Owner->GetInventory();


			for (auto yarrak : Inventory->GetItems())
			{
				UE_LOG(LogTemp, Warning, TEXT("item: %d, AMOUNT %d"), yarrak.Key, yarrak.Value);
			}


			if (Inventory)
			{
				auto Items = Inventory->GetItems();
				for (auto item : Items)
				{
					// If item amount is 0 or less(should never be less than 0) just skip to next item in player inventory
					if (item.Value <= 0) continue;

					//item.key = ItemID, item.value = amount of that item
					auto BarItem = CreateWidget<UUIInventoryItem>((APlayerController*)Player->GetController(), InvItemClass);
					BarItem->Owner = Owner;
					BarItem->ItemID = item.Key;
					BarItem->SetupInventoryItemCell();

					// WrapBox assigned from blueprint
					WrapBox->AddChild(BarItem);
				}

			}

		}
		
	}

}

void UUIPlayerInventory::Refresh()
{
	WrapBox->ClearChildren();
	AddItems();
}