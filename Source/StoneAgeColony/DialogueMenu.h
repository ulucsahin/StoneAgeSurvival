// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SurvivalWidget.h"
#include "DialogueMenu.generated.h"

class UTextBlock;

UCLASS()
class STONEAGECOLONY_API UDialogueMenu : public USurvivalWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "DialogueText")
	UTextBlock* DialogueTextBox;

	FString DialogueText;

protected:
	virtual void InitialSetup() override;

	
};
