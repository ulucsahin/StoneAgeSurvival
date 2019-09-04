// Fill out your copyright notice in the Description page of Project Settings.

#include "CraftListButton.h"
#include "ObjectFactory.h"
#include "StoneAgeColonyCharacter.h"
#include "CraftingStationMenu.h"
#include "UsableActor.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"

UCraftListButton::UCraftListButton(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UCraftListButton::CreateRequiredObjects()
{
	/* Should be called OnButtonClick and OnButtonHover only. No need to call in other functions. */

	if (Factory == nullptr)
	{
		Factory = NewObject<AObjectFactory>();
		//Factory->AddToRoot();
	}
	if (RepresentedItem == nullptr)
	{
		RepresentedItem = Factory->CreateObjectBetter(ItemID);
		//RepresentedItem->AddToRoot();
	}
	
}

void UCraftListButton::OnButtonClick()
{
	CreateRequiredObjects();

	// Send the item this button represents to BelongingStationMenu and start crafting
	BelongingStationMenu->ReceiveInformationFromButton(RepresentedItem, ItemID); //ItemID creates problems when taken from RepresentedItem(my bad)
}

void UCraftListButton::OnButtonHover()
{
	CreateRequiredObjects();
	SetDescriptionText();
	SetRequirementsText();
}

void UCraftListButton::SetStationMenu(UCraftingStationMenu* StationMenu)
{
	this->BelongingStationMenu = StationMenu;
}

void UCraftListButton::SetDescriptionText()
{
	FText Description = RepresentedItem->Description; 
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