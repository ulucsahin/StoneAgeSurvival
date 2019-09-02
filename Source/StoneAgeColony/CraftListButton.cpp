// Fill out your copyright notice in the Description page of Project Settings.

#include "CraftListButton.h"
#include "ObjectFactory.h"
#include "StoneAgeColonyCharacter.h"
#include "CraftingStationMenu.h"
#include "UsableActor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"

UCraftListButton::UCraftListButton(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CraftAmount = 1;// TODO: Craft how many?
}

void UCraftListButton::CreateRequiredObjects()
{
	/* Should be called OnButtonClick and OnButtonHover only. No need to call in other functions. */

	if (Factory == nullptr)
	{
		Factory = NewObject<AObjectFactory>();
	}
	if (RepresentedItem == nullptr)
	{
		RepresentedItem = Factory->CreateObjectBetter(ItemID);
	}
	
}

void UCraftListButton::OnButtonClick()
{
	CreateRequiredObjects();
	UE_LOG(LogTemp, Warning, TEXT("UCraftListButton::OnButtonClick ?????????"));
	if (CraftingRequirementMet())
	{
		UE_LOG(LogTemp, Warning, TEXT("UCraftListButton::OnButtonClick Craft item hehe xd"));
		AStoneAgeColonyCharacter* Player = (AStoneAgeColonyCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

		if (Player)
		{
			auto PlayerInventory = Player->GetInventory();

			int a = 1;
			// Consume items from player inventory
			for (auto Requirement : RepresentedItem->CraftRequirements)
			{
				int32 UsedItemID = Requirement.Key;
				int32 ConsumedAmount = Requirement.Value * CraftAmount;
				UE_LOG(LogTemp, Warning, TEXT("UCraftListButton::OnButtonClick UsedItemID %d"), UsedItemID);
				UE_LOG(LogTemp, Warning, TEXT("UCraftListButton::OnButtonClick ConsumedAmount %d"), ConsumedAmount);
				UE_LOG(LogTemp, Warning, TEXT("UCraftListButton::OnButtonClick Player Has: %d"), PlayerInventory[UsedItemID]);
				//PlayerInventory.Emplace(UsedItemID, PlayerInventory[UsedItemID] - ConsumedAmount);
				Player->ConsumeItemFromInventory(UsedItemID, ConsumedAmount);
				UE_LOG(LogTemp, Warning, TEXT("UCraftListButton::OnButtonClick Player Has: %d"), PlayerInventory[UsedItemID]);
			}

			Player->AddToInventory(ItemID, CraftAmount);
		}
	}

}

void UCraftListButton::OnButtonHover()
{
	CreateRequiredObjects();
	SetDescriptionText();
	SetRequirementsText();
}

bool UCraftListButton::CraftingRequirementMet() 
{
	/* Checks if required items are in player inventory for crafting the item of this button. */

	bool RequirementsMet = true;
	AStoneAgeColonyCharacter* Player = (AStoneAgeColonyCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (Player)
	{
		auto PlayerInventory = Player->GetInventory();

		for (auto Requirement : RepresentedItem->CraftRequirements)
		{
			int32 RequiredItem = Requirement.Key;
			int32 RequiredAmount = Requirement.Value * CraftAmount;
			
			//UE_LOG(LogTemp, Warning, TEXT("UCraftListButton::CraftingRequirementMet RequiredItem: %d"), RequiredItem);
			//UE_LOG(LogTemp, Warning, TEXT("UCraftListButton::CraftingRequirementMet Required Amount: %d"), RequiredAmount);
			
			for (auto asd : PlayerInventory)
			{
				//UE_LOG(LogTemp, Warning, TEXT("UCraftListButton::CraftingRequirementMet Player Items: %d Amount: %d"), asd.Key, asd.Value);
			}

			// if player don't have enough of this item
			if (PlayerInventory.Contains(RequiredItem))
			{
				//UE_LOG(LogTemp, Warning, TEXT("UCraftListButton::CraftingRequirementMet Player has: %d"), PlayerInventory[RequiredItem]);
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

void UCraftListButton::SetStationMenu(UCraftingStationMenu* StationMenu)
{
	this->BelongingStationMenu = StationMenu;
}

void UCraftListButton::SetDescriptionText()
{
	FText Description = RepresentedItem->Description; //FText::FromString("hehe XD");
	BelongingStationMenu->DescriptionText->SetText(Description);
}

void UCraftListButton::SetRequirementsText()
{
	FString RepresentedItemName = Factory->GetObjectNameFromID(ItemID);

	FString Requirements = "Required items for crafting " + RepresentedItemName + ":\n";
	for (auto Requirement : RepresentedItem->CraftRequirements)
	{
		int32 RequiredItem = Requirement.Key;
		FString RequiredAmount = FString::FromInt(Requirement.Value);

		auto RequiredItemName = Factory->GetObjectNameFromID(RequiredItem);

		Requirements.Append(RequiredItemName + ": " + RequiredAmount + "\n");
	}

	FText Requirements_ = FText::FromString(Requirements);
	BelongingStationMenu->RequirementsText->SetText(Requirements_);
} 