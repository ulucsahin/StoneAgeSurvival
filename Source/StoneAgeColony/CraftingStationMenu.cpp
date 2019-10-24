// Fill out your copyright notice in the Description page of Project Settings.

#include "CraftingStationMenu.h"
#include "CraftingStation.h"
#include "UsableActor.h"
#include "UObject/ConstructorHelpers.h"
#include "ObjectFactory.h"
#include "CraftListButton.h"
#include "StoneAgeColonyCharacter.h"
#include "Structure.h"
#include "CraftingStation.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/UMG/Public/Components/VerticalBox.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/ProgressBar.h"
#include "Runtime/Engine/Classes/Components/TimelineComponent.h"
#include "Runtime/Engine/Classes/Curves/CurveFloat.h"
#include "Runtime/Engine/Public/TimerManager.h"

UCraftingStationMenu::UCraftingStationMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	//PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UCraftListButton> BPClass(TEXT("'/Game/Uluc/Settlement/CraftingStations/MenuAssets/CraftingListButton.CraftingListButton_C'"));
	CraftMenuItem = BPClass.Class;

	CraftAmount = 1; // TODO: Craft how many?
}

// Called in blueprint
void UCraftingStationMenu::RegisterToPlayer(AStoneAgeColonyCharacter* Player)
{
	this->Player = Player;
}

void UCraftingStationMenu::InitialSetup()
{
	// Get most recent information from belonging structure 
	UpdateProgressBar(((ACraftingStation*)OwnerStructure)->CraftingProgress);
	CraftingProgressBar->SetVisibility(ESlateVisibility::Hidden);
}

// Called in blueprint
void UCraftingStationMenu::AddItems(int32 CraftStationID)
{
	for (int32 Item : ((ACraftingStation*)OwnerStructure)->CraftableItems)
	{
		auto CraftListButton = CreateWidget<UCraftListButton>((APlayerController*)Player->GetController(), CraftMenuItem);
		CraftListButton->ItemID = Item;
		CraftListButton->SetupInventoryItemCell();
		CraftListButton->SetStationMenu(this);

		// WrapBox assigned from blueprint
		VerticalBox->AddChild(CraftListButton);
	}
}

void UCraftingStationMenu::ReceiveInformationFromButton(AUsableActor* RepresentedItem, int32 ItemID)
{
	// wtf are those casts

	// do nothing if currently crafting
	if(!((ACraftingStation*)OwnerStructure)->CurrentlyCrafting)
	{
		((ACraftingStation*)OwnerStructure)->CurrentItem = RepresentedItem;
		((ACraftingStation*)OwnerStructure)->CurrentItemID = ItemID;
		((ACraftingStation*)OwnerStructure)->SetCraftingCharacter(Player);
		/*this->CurrentItem = RepresentedItem;
		this->CurrentItemID = ItemID;*/
		if (((ACraftingStation*)OwnerStructure)->CraftingRequirementsMet())
		{
			
			((ACraftingStation*)OwnerStructure)->StartCrafting(5.f); // Crafting time should be different for items.
		}
	}
	
}

void UCraftingStationMenu::UpdateProgressBar(float Percent)
{
	CraftingProgressBar->SetPercent(Percent);
}

void UCraftingStationMenu::SetProgressBarVisibility(bool Visibility)
{
	if (Visibility == false)
	{
		CraftingProgressBar->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		CraftingProgressBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void UCraftingStationMenu::CloseMenu()
{
	//StopCrafting();
	IsActive = false;
	RemoveFromParent();

	auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
		PlayerController->bEnableClickEvents = false;
		PlayerController->bEnableMouseOverEvents = false;
	}
}