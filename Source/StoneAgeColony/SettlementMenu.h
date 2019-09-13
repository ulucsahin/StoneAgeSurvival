// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SurvivalWidget.h"
#include "SettlementMenu.generated.h"

//class UCraftListButton;
//class AStoneAgeColonyCharacter;
class UTextBlock;
//class UVerticalBox;
//class UProgressBar;
//class UTimelineComponent;
//class UCurveFloat;


UCLASS()
class STONEAGECOLONY_API USettlementMenu : public USurvivalWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Cpp")
	UTextBlock* SettlementNameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Cpp")
	UTextBlock* SettlementStatsText;

	virtual void InitialSetup() override;
};
