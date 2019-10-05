// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogueChoiceButton.h"
#include "DialogueMenu.h"
#include "SettlementMember.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"

void UDialogueChoiceButton::InitialSetup()
{
	UE_LOG(LogTemp, Warning, TEXT("UDialogueChoiceButton::InitialSetup"));


	ChoiceTextBox->SetText(FText::FromString(Data->Text));

}

void UDialogueChoiceButton::OnButtonClick()
{
	UE_LOG(LogTemp, Warning, TEXT("UDialogueChoiceButton::OnButtonClick"));
}

void UDialogueChoiceButton::OnButtonHover()
{
	UE_LOG(LogTemp, Warning, TEXT("UDialogueChoiceButton::OnButtonHover"));
}

void UDialogueChoiceButton::SetOwnerMemberProfession()
{
	ASettlementMember* OwnerMember = OwnerDialogueMenu->Owner;
	OwnerMember->Profession = "yarrak";

}