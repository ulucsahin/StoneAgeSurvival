// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIItemSlot.h"
#include "CraftListButton.generated.h"

class UCraftingStationMenu;
class AUsableActor;
class AObjectFactory;

UCLASS()
class STONEAGECOLONY_API UCraftListButton : public UUIItemSlot
{
	GENERATED_BODY()
	
public:

	UCraftListButton(const FObjectInitializer& ObjectInitializer);

	void CreateRequiredObjects();

	UFUNCTION(BlueprintCallable, Category = "ButtonMouseEvents")
	void OnButtonClick();

	UFUNCTION(BlueprintCallable, Category = "ButtonMouseEvents")
	void OnButtonHover();

	bool CraftingRequirementMet();

	void SetStationMenu(UCraftingStationMenu* StationMenu);

	void SetDescriptionText();
	void SetRequirementsText();

private:
	UCraftingStationMenu* BelongingStationMenu;
	TMap<int32, int32> RequiredItems; // Item ID, Item Amount map. amount of item required with id.
	AUsableActor* RepresentedItem;
	AObjectFactory* Factory;
	int32 CraftAmount;

};



	