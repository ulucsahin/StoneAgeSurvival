// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogueChoiceButton.h"
#include "DialogueMenu.h"
#include "SettlementMember.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/Engine/Classes/Engine/StreamableManager.h"
#include "Runtime/Engine/Classes/Engine/AssetManager.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Internationalization/Regex.h"

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
	else if (Type == "ask_job")
	{
		return Response + OwnerDialogueMenu->Owner->Profession + ".";
	}
	else if (Type == "assign_job")
	{
		// assign job to owner settlement member
		OwnerDialogueMenu->Owner->Profession = GetJobFromQuery();
		return Response;
	}
	else
	{
		return "";
	}
	
}

FString UDialogueChoiceButton::GetJobFromQuery()
{
	const FRegexPattern myPattern(TEXT("^[a-z,A-Z]+"));
	FRegexMatcher myMatcher(myPattern, Query);

	if (myMatcher.FindNext())
	{
		int32 b = myMatcher.GetMatchBeginning();
		int32 e = myMatcher.GetMatchEnding();
		UE_LOG(LogTemp , Warning, TEXT("REGEX %s"), *Query.Mid(b, e));
		return Query.Mid(b, e);
	}
	else
	{
		return "";
	}
	
}