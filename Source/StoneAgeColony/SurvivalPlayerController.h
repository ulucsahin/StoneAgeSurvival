// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SurvivalWidget.h"

#include "SurvivalPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STONEAGECOLONY_API ASurvivalPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlayingState() override;

	USurvivalWidget* UserInterface;


};
