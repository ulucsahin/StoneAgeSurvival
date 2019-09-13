// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SurvivalWidget.h"
#include "HouseMenu.generated.h"

class UTextBlock;

UCLASS()
class STONEAGECOLONY_API UHouseMenu : public USurvivalWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "HouseProperties")
	UTextBlock* RequirementsText;

	virtual void InitialSetup() override;

private:
	int32 asdasdasd;
};
