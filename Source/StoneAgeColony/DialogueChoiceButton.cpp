// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogueChoiceButton.h"
#include "DialogueMenu.h"
#include "SettlementMember.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/Engine/Classes/Engine/StreamableManager.h"
#include "Runtime/Engine/Classes/Engine/AssetManager.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

void UDialogueChoiceButton::InitialSetup()
{
	PropertiesDataTable = OwnerDialogueMenu->Owner->PropertiesDataTable;

}

void UDialogueChoiceButton::SetupType(FString ID_as_String)
{
	const FString ContextString(TEXT("Button Type Context"));
	auto Data = PropertiesDataTable->FindRow<FLinkedListDialogueItem>(FName(*ID_as_String), ContextString, true);
	ID = Data->ID;
	Type = Data->Type;
	Query = Data->Query;
	Response = Data->Response;
	Next = Data->Next;
	Previous = Data->Previous;
	
	ChoiceTextBox->SetText(FText::FromString(Query));
}

void UDialogueChoiceButton::OnButtonClick()
{
	OwnerDialogueMenu->SetText(GenerateResponse());
	OwnerDialogueMenu->AddChoices(Next);

}

void UDialogueChoiceButton::OnButtonHover()
{
	
}

void UDialogueChoiceButton::SetOwnerMemberProfession()
{
	ASettlementMember* OwnerMember = OwnerDialogueMenu->Owner;
	OwnerMember->Profession = "yarrak";
}

// this is retarded
FString UDialogueChoiceButton::GenerateResponse()
{
	if (Type == "introduction")
	{
		return Response + OwnerDialogueMenu->Owner->Name + ".";
	}
	else if (Type == "asking_job")
	{
		return Response + OwnerDialogueMenu->Owner->Profession + ".";
	}
	else
	{
		return "";
	}
	
}