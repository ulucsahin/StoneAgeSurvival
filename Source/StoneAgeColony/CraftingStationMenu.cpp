// Fill out your copyright notice in the Description page of Project Settings.

#include "CraftingStationMenu.h"
#include "CraftingStation.h"
#include "UsableActor.h"
#include "UObject/ConstructorHelpers.h"
#include "ObjectFactory.h"
#include "CraftListButton.h"
#include "StoneAgeColonyCharacter.h"
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

	CraftingProgressBar->SetVisibility(ESlateVisibility::Hidden);
}

// Called in blueprint
void UCraftingStationMenu::AddItems(int32 CraftStationID)
{
	AObjectFactory* Factory = NewObject<AObjectFactory>();
	auto BelongingStationInstance = (ACraftingStation*) Factory->CreateObjectBetter(CraftStationID);
		
	for (int32 Item : BelongingStationInstance->CraftableItems)
	{

		UE_LOG(LogTemp, Warning, TEXT("UCraftingStationMenu::AddItems"));
		
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
	// do nothing if currently crafting
	if(!CurrentlyCrafting)
	{
		this->CurrentItem = RepresentedItem;
		this->CurrentItemID = ItemID;
		if (CraftingRequirementsMet())
		{
			if (Player)
			{
				auto PlayerInventory = Player->GetInventory();

				// Consume items from player inventory
				for (auto Requirement : RepresentedItem->CraftRequirements)
				{
					int32 UsedItemID = Requirement.Key;
					int32 ConsumedAmount = Requirement.Value * CraftAmount;
					Player->ConsumeItemFromInventory(UsedItemID, ConsumedAmount);
				}

				StartCrafting(5.f); // Crafting time should be different for items.
			}
		}
	}
	
}

bool UCraftingStationMenu::CraftingRequirementsMet()
{
	/* Checks if required items are in player inventory for crafting the item of this button. */

	bool RequirementsMet = true;

	if (Player)
	{
		auto PlayerInventory = Player->GetInventory();

		for (auto Requirement : CurrentItem->CraftRequirements)
		{
			int32 RequiredItem = Requirement.Key;
			int32 RequiredAmount = Requirement.Value * CraftAmount;

			// if player don't have enough of this item
			if (PlayerInventory.Contains(RequiredItem))
			{
				if (PlayerInventory[RequiredItem] < RequiredAmount)
				{
					RequirementsMet = false;
					break;
				}
			}
			else
			{
				RequirementsMet = false;
			}

		}

	}

	return RequirementsMet;
}

void UCraftingStationMenu::StartCrafting(float CraftingTime)
{
	StartUpdatingProgressBar(CraftingTime);
	CraftingProgressBar->SetVisibility(ESlateVisibility::Visible);
	CurrentlyCrafting = true;
}

void UCraftingStationMenu::StartUpdatingProgressBar(float CraftingTime)
{
	FTimerDelegate TimerDel;

	//Binding the function with specific values
	float UpdateFrequency = 0.1f;
	TimerDel.BindUFunction(this, FName("UpdateProgressBar"), CraftingTime, UpdateFrequency);
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, 0.1f, true);
}

void UCraftingStationMenu::UpdateProgressBar(float CraftingTime, float UpdateFrequency)
{
	CraftingBarProgress += 1 / CraftingTime * UpdateFrequency;
	CraftingProgressBar->SetPercent(CraftingBarProgress);
	
	// Crafting Completed
	if (CraftingBarProgress >= 1.f)
	{
		CraftingBarProgress = 0.f;
		CraftingProgressBar->SetPercent(0.f);
		CraftingProgressBar->SetVisibility(ESlateVisibility::Hidden);
		Player->AddToInventory(CurrentItemID, CraftAmount * CurrentItem->YieldAmount);
		CurrentlyCrafting = false;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}

}

// Called in BP
void UCraftingStationMenu::CloseMenu()
{
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