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
}

void UUISpecifyCrafting::InitializeCraftListItems()
{
	UE_LOG(LogTemp, Warning, TEXT("UUISpecifyCrafting::InitializeCraftListItems"));

	AObjectFactory* Factory = NewObject<AObjectFactory>();

	if (OwnerDialogueMenu)
	{
		ASettlementMember* OwnerMember = OwnerDialogueMenu->Owner;

		int32 WorkstationID = OwnerMember->Profession.WorkstationTypeID;
		if (WorkstationID != -1) // -1 means this settlement member has no workstation
		{
			ACraftingStation* Station = Cast<ACraftingStation>(Factory->CreateObjectBetter(OwnerMember->Profession.WorkstationTypeID));
			UE_LOG(LogTemp, Warning, TEXT("1"));
			if (Station)
			{
				UE_LOG(LogTemp, Warning, TEXT("2"));
				for (auto Item : Station->CraftableItems)
				{
					UE_LOG(LogTemp, Warning, TEXT("3"));
					auto Controller = (APlayerController*)OwnerDialogueMenu->Player->GetController();
					UE_LOG(LogTemp, Warning, TEXT("4"));
					USpecifyCraftListItem* CraftListItem = CreateWidget<USpecifyCraftListItem>(Controller, ListItemWidget);
					UE_LOG(LogTemp, Warning, TEXT("5"));
					VerticalBox->AddChild(CraftListItem);
					UE_LOG(LogTemp, Warning, TEXT("6123"));
				}

			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("anan xd"));
			}

			

		}
		
	}	

}