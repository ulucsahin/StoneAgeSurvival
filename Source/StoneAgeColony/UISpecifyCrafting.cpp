// Fill out your copyright notice in the Description page of Project Settings.

#include "UISpecifyCrafting.h"
#include "SpecifyCraftListItem.h"
#include "SettlementMember.h"
#include "DialogueMenu.h"
#include "ObjectFactory.h"
#include "CraftingStation.h"
#include "StoneAgeColonyCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/UMG/Public/Components/VerticalBox.h"


UUISpecifyCrafting::UUISpecifyCrafting(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<USpecifyCraftListItem> BPClass(TEXT("'/Game/Uluc/NPC/DialogueSystem/SpecifyCraftListItem_BP.SpecifyCraftListItem_BP_C'"));
	ListItemWidget = BPClass.Class;
}


void UUISpecifyCrafting::InitialSetup()
{
	UE_LOG(LogTemp, Warning, TEXT("UUISpecifyCrafting::InitialSetup"));
	
}


void UUISpecifyCrafting::CloseMenu()
{
	IsActive = false;
	RemoveFromParent();
	OwnerDialogueMenu->SetVisibility(ESlateVisibility::Visible);
	OwnerDialogueMenu->Owner->SpecifyCratingMenuOn = false;
	OwnerDialogueMenu->SetChoiceButtonsEnabled(true);
	OwnerDialogueMenu->Owner->CraftList = ConstructCraftList();
	// Give CraftList to Owner SettlementMember
	//OwnerDialogueMenu->Owner->CraftList =
}

void UUISpecifyCrafting::InitializeCraftListItems()
{
	UE_LOG(LogTemp, Warning, TEXT("UUISpecifyCrafting::InitializeCraftListItems"));

	AObjectFactory* Factory = NewObject<AObjectFactory>();
	CraftListItems.Empty();

	if (OwnerDialogueMenu)
	{
		ASettlementMember* OwnerMember = OwnerDialogueMenu->Owner;

		int32 WorkstationID = OwnerMember->Profession.WorkstationTypeID;
		if (WorkstationID != -1) // -1 means this settlement member has no workstation
		{
			ACraftingStation* Station = Cast<ACraftingStation>(Factory->CreateObjectBetter(OwnerMember->Profession.WorkstationTypeID));
			if (Station)
			{
				for (auto ItemID : Station->CraftableItems)
				{
					auto Controller = (APlayerController*)OwnerDialogueMenu->Player->GetController();
					USpecifyCraftListItem* CraftListItem = CreateWidget<USpecifyCraftListItem>(Controller, ListItemWidget);
					CraftListItem->RepresentedItemID = ItemID;
					CraftListItem->OwnerSpecifyCraftingMenu = this;
					VerticalBox->AddChild(CraftListItem);
					CraftListItems.Add(CraftListItem);
				}

			}		

		}

		//OwnerMember->CraftList = ConstructCraftList();		
	}	

	
}

TMap<int32, int32> UUISpecifyCrafting::ConstructCraftList()
{
	TMap<int32, int32> CraftList = {};
	for (auto Item : CraftListItems)
	{
		CraftList.Emplace(Item->RepresentedItemID, Item->CraftAmount);
	}

	return CraftList;
}