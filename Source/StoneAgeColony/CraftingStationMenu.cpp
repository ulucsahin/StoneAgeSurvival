// Fill out your copyright notice in the Description page of Project Settings.

#include "CraftingStationMenu.h"
#include "CraftingStation.h"
#include "UObject/ConstructorHelpers.h"
#include "ObjectFactory.h"
#include "CraftListButton.h"
#include "StoneAgeColonyCharacter.h"
#include "Runtime/UMG/Public/Components/VerticalBox.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"

UCraftingStationMenu::UCraftingStationMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UCraftListButton> BPClass(TEXT("'/Game/Uluc/Settlement/CraftingStations/MenuAssets/CraftingListButton.CraftingListButton_C'"));
	CraftMenuItem = BPClass.Class;
}

// Called in blueprint
void UCraftingStationMenu::RegisterToPlayer(AStoneAgeColonyCharacter* Player)
{
	this->Player = Player;
	//Player->UIPlayerInventory = this;
}

// Called in blueprint
void UCraftingStationMenu::AddItems(int32 CraftStationID)
{
	AObjectFactory* Factory = NewObject<AObjectFactory>();
	auto BelongingStationInstance = (ACraftingStation*) Factory->CreateObjectBetter(CraftStationID);
		
	for (int32 Item : BelongingStationInstance->CraftableItems)
	{

		UE_LOG(LogTemp, Warning, TEXT("UCraftingStationMenu::AddItems."));
		
		auto CraftListButton = CreateWidget<UCraftListButton>((APlayerController*)Player->GetController(), CraftMenuItem);
		CraftListButton->ItemID = Item;
		CraftListButton->SetupInventoryItemCell();
		CraftListButton->SetStationMenu(this);

		// WrapBox assigned from blueprint
		VerticalBox->AddChild(CraftListButton);
	}
}