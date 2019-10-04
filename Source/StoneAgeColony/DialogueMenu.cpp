// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogueMenu.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"

void UDialogueMenu::InitialSetup()
{
	UE_LOG(LogTemp, Warning, TEXT("DialogueMenu::InitialSetup"));
	DialogueText = "I gotchu homie!";

	FText Dialogue_ = FText::FromString(DialogueText);
	DialogueTextBox->SetText(Dialogue_);
}