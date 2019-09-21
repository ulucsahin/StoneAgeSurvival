// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlantProgressBar.generated.h"


class APlant;

UCLASS()
class STONEAGECOLONY_API UPlantProgressBar : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// Binded in blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
	float Percentage;

};
