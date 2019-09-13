// Fill out your copyright notice in the Description page of Project Settings.

#include "SettlementMenu.h"
#include "Settlement.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"

void USettlementMenu::InitialSetup()
{
	if (OwnerSettlement)
	{
		// Set SettlementName TextBox text
		auto SettlementName = OwnerSettlement->Name;
		SettlementNameText->SetText(FText::FromString(SettlementName));

		// Set SettlementStats TextBox text
		FString SettlementStats = "";
		SettlementStats += "Population Limit: " + FString::FromInt(OwnerSettlement->PopulationLimit);
		SettlementStats += "\nLevel: " + FString::FromInt(OwnerSettlement->Level);
		SettlementStats += "\nExperience: " + FString::FromInt(OwnerSettlement->Experience);
		SettlementStats += "\nBuildingLimit: " + FString::FromInt(OwnerSettlement->BuildingLimit);
		SettlementStats += "\nNumber of Structures: " + FString::FromInt(OwnerSettlement->Structures.Num());
		SettlementStatsText->SetText(FText::FromString(SettlementStats));
	}
	
		
}
