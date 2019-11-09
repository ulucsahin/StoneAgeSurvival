// Fill out your copyright notice in the Description page of Project Settings.

#include "SpecifyCraftListItem.h"
#include "UISpecifyCrafting.h"
#include "Components/Slider.h"
#include "SettlementMember.h"
#include "DialogueMenu.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "ObjectFactory.h"
#include "CraftingStation.h"
#include "Inventory.h"

void USpecifyCraftListItem::InitialSetup()
{
	/* Called in blueprint */

	//ItemNameBlock->SetText(FText::FromString("ItemNameHere"));
	AObjectFactory* Factory = NewObject<AObjectFactory>();

	OwnerMember = OwnerSpecifyCraftingMenu->OwnerDialogueMenu->Owner;
	OwnerInventory = OwnerMember->GetInventory();
	RepresentedItemInstance = Factory->CreateObjectBetter(RepresentedItemID);

	//int32 WorkstationTypeID = OwnerMember->Profession.WorkstationTypeID;
	//ACraftingStation* Station = Cast<ACraftingStation>(Factory->CreateObjectBetter(WorkstationTypeID));
	
	auto ItemName = Factory->GetObjectNameFromID(RepresentedItemID);
	ItemNameBlock->SetText(FText::FromString(ItemName));
	ItemAmountBlock->SetText(FText::FromString("0"));
	
	//Slider->value

}

// Called from blueprint
void USpecifyCraftListItem::OnSliderValueChanged()
{
	//UE_LOG(LogTemp, Warning, TEXT("USpecifyCraftListItem::OnSliderValueChanged %f"), Slider->GetValue());
//	UE_LOG(LogTemp, Warning, TEXT("USpecifyCraftListItem::OnSliderValueChanged %d"), RepresentedItemID);


	
	CraftAmount = CalculateCraftAmountFromSlider();
	ItemAmountBlock->SetText(FText::FromString(FString::FromInt(CraftAmount)));
}

int32 USpecifyCraftListItem::CalculateCraftAmountFromSlider()
{
	/* Calculates amount depending on requirements of item this object represents and amount of materials in owner member's inventory
	*  Floors the value.
	*/

	int32 Result = 99999;
	int32 AmountToCraft = 0;
	int32 RequiredItemID = 0;
	int32 RequiredItemAmount = 0;
	TMap<int32, int32> Items = OwnerInventory->GetItems();

	for (auto RequiredItem : RepresentedItemInstance->CraftRequirements)
	{
		RequiredItemID = RequiredItem.Key;
		RequiredItemAmount = RequiredItem.Value;
		
		if (Items.Contains(RequiredItemID))
		{
			AmountToCraft = Items[RequiredItemID] / RequiredItemAmount;

			if (AmountToCraft < Result) Result = AmountToCraft; // lowest ratio item decides how many we can craft

		}
		else
		{
			Result = 0;
		}
	}

	Result *= Slider->GetValue();

	
	//UE_LOG(LogTemp, Warning, TEXT("AmountToCraft %d"), AmountToCraft);
	UE_LOG(LogTemp, Warning, TEXT("Result %d"), Result);
	//UE_LOG(LogTemp, Warning, TEXT("RequiredItemID %d"), RequiredItemID);
	//UE_LOG(LogTemp, Warning, TEXT("RequiredItemAmount %d"), RequiredItemAmount);

	//for (auto item : Items)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Item: %d , Amount: %d"), item.Key, item.Value);
	//}

	return Result;
}