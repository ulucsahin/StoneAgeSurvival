// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogueMenu.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "SettlementMember.h"
#include "DialogueChoiceButton.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "StoneAgeColonyCharacter.h"
#include "SettlementMemberAI.h"
#include "Runtime/UMG/Public/Components/VerticalBox.h"

UDialogueMenu::UDialogueMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UDialogueChoiceButton> BPClass(TEXT("'/Game/Uluc/NPC/DialogueSystem/DialogueChoiceButton_BP.DialogueChoiceButton_BP_C'"));
	DialogueChoiceButtonWidget = BPClass.Class;
}

void UDialogueMenu::InitialSetup()
{
	Player = (AStoneAgeColonyCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	AddChoices(StartingChoiceIDs); // currently only with ID 20000.
}

void UDialogueMenu::CloseMenu()
{
	Super::CloseMenu();
	auto Controller = Owner->AIController;
	if (Controller)
	{
		Controller->Activity = EActivity::VE_Idle;
	}

	Owner->Act();

	//Owner->AIController->Activity = EActivity::VE_Idle;

}

void UDialogueMenu::AddChoices(TArray<int32> ChoiceIDs)
{
	ChoicesVerticalBox->ClearChildren();

	auto Controller = (APlayerController*)Player->GetController();

	for (auto ID : ChoiceIDs)
	{
		auto DialogueChoiceButton = CreateWidget<UDialogueChoiceButton>(Controller, DialogueChoiceButtonWidget);
		DialogueChoiceButton->OwnerDialogueMenu = this;
		ChoicesVerticalBox->AddChild(DialogueChoiceButton); // WrapBox assigned from blueprint
		DialogueChoiceButton->InitialSetup();
		DialogueChoiceButton->SetupType(*FString::FromInt(ID));
	}
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