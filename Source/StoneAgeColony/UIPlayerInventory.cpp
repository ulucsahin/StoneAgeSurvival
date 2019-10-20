// Fill out your copyright notice in the Description page of Project Settings.

#include "UIPlayerInventory.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/UMG/Public/Components/WrapBox.h"
#include "UIInventoryItem.h"
#include "Inventory.h"
#include "StoneAgeColonyCharacter.h"
//#include "Runtime/UMG/Public/Blueprint/WidgetTree.h"

UUIPlayerInventory::UUIPlayerInventory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUIInventoryItem> BPClass(TEXT("'/Game/Uluc/HUD/Inventory/InventoryItem.InventoryItem_C'"));
	InvItemClass = BPClass.Class;
}

void UUIPlayerInventory::Test()
{
	//auto root_widget = WidgetTree->ConstructWidget<UWrapBox>(UWrapBox::StaticClass(), TEXT("RootWidget"));
}

// Called in blueprint
void UUIPlayerInventory::RegisterToPlayer(AStoneAgeColonyCharacter* Player)
{
	this->Player = Player;
	Player->UIPlayerInventory = this;
}


// Called in blueprint
void UUIPlayerInventory::AddItems()
{
	for (auto item : Player->GetInventory()->GetItems())
	{
		// If item amount is 0 or less(should never be less than 0) just skip to next item in player inventory
		if (item.Value <= 0) continue;

		//item.key = ItemID, item.value = amount of that item
		auto BarItem = CreateWidget<UUIInventoryItem>((APlayerController*)Player->GetController(), InvItemClass);
		BarItem->ItemID = item.Key;
		BarItem->SetupInventoryItemCell();

		// WrapBox assigned from blueprint
		WrapBox->AddChild(BarItem);
	}
}

void UUIPlayerInventory::Refresh()
{
	WrapBox->ClearChildren();
	AddItems();
}