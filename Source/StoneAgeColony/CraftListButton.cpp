// Fill out your copyright notice in the Description page of Project Settings.

#include "CraftListButton.h"
#include "ObjectFactory.h"
#include "StoneAgeColonyCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void UCraftListButton::OnButtonClick()
{
	UE_LOG(LogTemp, Warning, TEXT("UCraftListButton::OnClick"));
	UE_LOG(LogTemp, Warning, TEXT("ItemID: %d"), ItemID);
	UE_LOG(LogTemp, Warning, TEXT("ItemName %s"), *ItemName);
	UE_LOG(LogTemp, Warning, TEXT("ItemAmount %d"), ItemAmount);

	if (CraftingRequirementMet())
	{
		UE_LOG(LogTemp, Warning, TEXT("Craft item hehe xD"));
		AStoneAgeColonyCharacter* Player = (AStoneAgeColonyCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

		if (Player)
		{
			Player->AddToInventory(ItemID, 1); // TODO: Craft how many?
		}
	}

}

bool UCraftListButton::CraftingRequirementMet() 
{
	/* Checks if required items are in player inventory for crafting the item of this button. */

	bool RequirementsMet = true;
	AStoneAgeColonyCharacter* Player = (AStoneAgeColonyCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (Player)
	{
		auto PlayerInventory = Player->GetInventory();

		AObjectFactory* Factory = NewObject<AObjectFactory>();
		auto Item = Factory->CreateObjectBetter(ItemID);

		for (auto Requirement : Item->CraftRequirements)
		{
			int32 RequiredItem = Requirement.Key;
			int32 RequiredAmount = Requirement.Value;
			
			// if player don't have enough of this item
			if (PlayerInventory.Contains(RequiredItem))
			{
				if (PlayerInventory[RequiredItem] >= RequiredAmount)
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