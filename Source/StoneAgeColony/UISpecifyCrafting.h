// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SurvivalWidget.h"
#include "UISpecifyCrafting.generated.h"

//class ACraftingStation;
//class ASettlementMember;
class UDialogueMenu;
class UVerticalBox;
class USpecifyCraftListItem;

/**
 * 
 */
UCLASS()
class STONEAGECOLONY_API UUISpecifyCrafting : public USurvivalWidget
{
	GENERATED_BODY()

public:
	UUISpecifyCrafting(const FObjectInitializer& ObjectInitializer);
	virtual void InitialSetup() override;
	virtual void CloseMenu() override;
	void InitializeCraftListItems();
	TMap<int32, int32> ConstructCraftList();

	UDialogueMenu* OwnerDialogueMenu;


	// UI Elements
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "VerticalBoxCpp")
	UVerticalBox* VerticalBox;


	TArray<USpecifyCraftListItem*> CraftListItems;
private:
	TSubclassOf<USpecifyCraftListItem> ListItemWidget;
};
