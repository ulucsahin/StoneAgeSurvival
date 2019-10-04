// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SurvivalWidget.h"
#include "DialogueMenu.generated.h"

/**
 * 
 */
UCLASS()
class STONEAGECOLONY_API UDialogueMenu : public USurvivalWidget
{
	GENERATED_BODY()
	
protected:
	virtual void InitialSetup() override;

private:
	FString DialogueText;
};
