// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogueMenu.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "SettlementMember.h"
#include "DialogueChoiceButton.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "StoneAgeColonyCharacter.h"
#include "Runtime/UMG/Public/Components/VerticalBox.h"

UDialogueMenu::UDialogueMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UDialogueChoiceButton> BPClass(TEXT("'/Game/Uluc/NPC/DialogueSystem/DialogueChoiceButton_BP.DialogueChoiceButton_BP_C'"));
	DialogueChoiceButtonWidget = BPClass.Class;

}

void UDialogueMenu::InitialSetup()
{
	UE_LOG(LogTemp, Warning, TEXT("DialogueMenu:: InitialSetup"));

	auto Player = (AStoneAgeColonyCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	auto DialogueChoiceButton = CreateWidget<UDialogueChoiceButton>((APlayerController*)Player->GetController(), DialogueChoiceButtonWidget);
	DialogueChoiceButton->OwnerDialogueMenu = this;

	// TEST
	FLinkedListDialogueItem test;
	test.ID = 0;
	test.Type = EButtonTypes::VE_String;
	test.Text = "Pepega suk mi pini";


	DialogueChoiceButton->Data = &test;
	
	// WrapBox assigned from blueprint
	ChoicesVerticalBox->AddChild(DialogueChoiceButton);
	DialogueChoiceButton->InitialSetup();
	


	//SetText("Hello! I am " + Owner->Name + ". I am currently " + Owner->Profession + "."); // Just for testing
}


void UDialogueMenu::SetText(FString Text)
{
	FText Dialogue_ = FText::FromString(Text);
	DialogueTextBox->SetText(Dialogue_);
}


//// Called in blueprint
//void UDialogueMenu::AddItems(int32 CraftStationID)
//{
//	for (int32 Item : ((ACraftingStation*)OwnerStructure)->CraftableItems)
//	{
//		auto CraftListButton = CreateWidget<UCraftListButton>((APlayerController*)Player->GetController(), CraftMenuItem);
//		CraftListButton->ItemID = Item;
//		CraftListButton->SetupInventoryItemCell();
//		CraftListButton->SetStationMenu(this);
//
//		// WrapBox assigned from blueprint
//		VerticalBox->AddChild(CraftListButton);
//	}
//}