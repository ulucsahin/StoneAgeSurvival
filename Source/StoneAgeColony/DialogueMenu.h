// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SurvivalWidget.h"
#include "DialogueMenu.generated.h"

class UTextBlock;
class UVerticalBox;
class ASettlementMember;
class UDialogueChoiceButton;
class AStoneAgeColonyCharacter;

UCLASS()
class STONEAGECOLONY_API UDialogueMenu : public USurvivalWidget
{
	GENERATED_BODY()
	
public:
	UDialogueMenu(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "DialogueText")
	UTextBlock* DialogueTextBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "DialogueChoices")
	UVerticalBox* ChoicesVerticalBox;

	ASettlementMember* Owner;

	virtual void InitialSetup() override;
	void AddChoices(TArray<int32> ChoiceIDs);
	void SetText(FString Text);
	virtual void CloseMenu() override;
	void SetChoiceButtonsEnabled(bool Enabled);

	TArray<int32> StartingChoiceIDs;
	AStoneAgeColonyCharacter* Player;
private:
	TSubclassOf<UDialogueChoiceButton> DialogueChoiceButtonWidget;
	
};
