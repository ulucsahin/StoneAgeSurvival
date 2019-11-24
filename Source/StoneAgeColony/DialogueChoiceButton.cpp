// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogueChoiceButton.h"
#include "DialogueMenu.h"
#include "SettlementMember.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/Engine/Classes/Engine/StreamableManager.h"
#include "Runtime/Engine/Classes/Engine/AssetManager.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Internationalization/Regex.h"
#include "SettlementMemberProfession.h"
#include "StoneAgeColonyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UISpecifyCrafting.h"
#include "UIPlayerInventory.h"


void UDialogueChoiceButton::InitialSetup()
{
	PropertiesDataTable = OwnerDialogueMenu->Owner->PropertiesDataTable;
}

void UDialogueChoiceButton::SetupType(FString ID_as_String)
{
	const FString ContextString(TEXT("Button Type Context"));
	auto Data = PropertiesDataTable->FindRow<FDialogueChoiceData>(FName(*ID_as_String), ContextString, true);
	ID = Data->ID;
	Type = Data->Type;
	Query = Data->Query;
	Payload = Data->Payload;
	Response = Data->Response;
	Next = Data->Next;
	Previous = Data->Previous;
	
	ChoiceTextBox->SetText(FText::FromString(Query));
}

void UDialogueChoiceButton::OnButtonClick()
{
	OwnerDialogueMenu->SetText(ActAndReturnResponse());
	OwnerDialogueMenu->AddChoices(Next);
	OwnerDialogueMenu->SetChoiceButtonsEnabled(NextButtonsEnabled);
}

void UDialogueChoiceButton::OnButtonHover()
{
	
}

void UDialogueChoiceButton::SetOwnerMemberProfession(EProfession Profession)
{
	auto NewProfession = USettlementMemberProfession::GetProfession(Profession);

	ASettlementMember* OwnerMember = OwnerDialogueMenu->Owner;
	OwnerMember->ChangeProfession(NewProfession);
}

FString UDialogueChoiceButton::ActAndReturnResponse()
{
	UE_LOG(LogTemp, Warning, TEXT("UDialogueChoiceButton:: ActAndReturnResponse after new windows"));
	if (Type == EButtonTypes::VE_Introduction)
	{
		return Response + OwnerDialogueMenu->Owner->Name + ".";
	}
	else if (Type == EButtonTypes::VE_AskJob)
	{
		return Response + OwnerDialogueMenu->Owner->Profession.ProfessionName + ".";
	}
	else if (Type == EButtonTypes::VE_AssignJob)
	{
		// assign job to owner settlement member
		SetOwnerMemberProfession(GetJobFromQuery());
		return Response;
	}
	else if (Type == EButtonTypes::VE_OpenMenu)
	{
		return "";
	}
	else if (Type == EButtonTypes::VE_OpenSpecifyCraftingMenu)
	{
		// VE_OpenSpecifyCraftingMenu is currently only for SpecifyCraftingMenu. For other menus different enums will be added.
		AStoneAgeColonyCharacter* Player = Cast<AStoneAgeColonyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (Player)
		{
			// Do not open if already on
			if (OwnerDialogueMenu->Owner->SpecifyCratingMenuOn == false)
			{
				OwnerDialogueMenu->Owner->SpecifyCratingMenuOn = true;
				NextButtonsEnabled = false;
				auto Menu = (UUISpecifyCrafting*)Player->OpenMenu(Payload, nullptr, nullptr);
				Menu->OwnerDialogueMenu = OwnerDialogueMenu;
				Menu->InitializeCraftListItems();
				OwnerDialogueMenu->SetVisibility(ESlateVisibility::Hidden);
			
			}

		}

		return "";
	}
	else if (Type == EButtonTypes::VE_OpenInventory)
	{
		AStoneAgeColonyCharacter* Player = Cast<AStoneAgeColonyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (Player)
		{
			auto InventoryMenu = (UUIPlayerInventory*)Player->OpenMenu(Payload, nullptr, nullptr);
			OwnerDialogueMenu->Owner->UIPlayerInventory = InventoryMenu;
			InventoryMenu->InitialSetup(OwnerDialogueMenu->Owner);
		}

		return "Here are my items.";
	}
	else
	{
		return "";
	}
	
}

EProfession UDialogueChoiceButton::GetJobFromQuery()
{
	static TMap<FString, EProfession> Professions = { 
		{"unoccupied", EProfession::VE_Unoccupied},
		{"carpenter", EProfession::VE_Carpenter},
		{"stoneworker", EProfession::VE_StoneWorker},
		{"farmer", EProfession::VE_Farmer},
		{"woodcutter", EProfession::VE_Woodcutter},
	};

	if (Professions.Contains(Payload))
		return Professions[Payload];
	else
		return EProfession::VE_Unoccupied;

	//const FRegexPattern myPattern(TEXT("^[a-z,A-Z,_, ]+"));
	//FRegexMatcher myMatcher(myPattern, Payload);

	//if (myMatcher.FindNext())
	//{
	//	int32 b = myMatcher.GetMatchBeginning();
	//	int32 e = myMatcher.GetMatchEnding();
	//	UE_LOG(LogTemp , Warning, TEXT("REGEX %s"), *Payload.Mid(b, e));

	//	//return Payload.Mid(b, e);
	//	return EProfession::VE_Unoccupied;;
	//}
	//else
	//{
	//	return EProfession::VE_Unoccupied;
	//}
	
}