// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIItemSlot.h"
#include "CraftListButton.generated.h"

/**
 * 
 */
UCLASS()
class STONEAGECOLONY_API UCraftListButton : public UUIItemSlot
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "MouseEvents2")
	void OnButtonClick();

	bool CraftingRequirementMet();
private:
	TMap<int32, int32> RequiredItems; // Item ID, Item Amount map. amount of item required with id.
};
