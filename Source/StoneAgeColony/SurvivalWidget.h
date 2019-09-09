// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
//#include "Runtime/UMG/Public/Components/Button.h"

#include "SurvivalWidget.generated.h"

class UButton;
class AStructure;

/**
 * 
 */
UCLASS()
class STONEAGECOLONY_API USurvivalWidget : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;


public:
	virtual void CloseMenu();
	bool IsActive;
	AStructure* OwnerStructure;
};
