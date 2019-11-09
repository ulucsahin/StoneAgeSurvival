// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SurvivalWidget.h"
#include "SpecifyCraftListItem.generated.h"

class USlider;
class UTextBlock;
class UUISpecifyCrafting;
class ASettlementMember;
class AUsableActor;
class UInventory;
class UEditableTextBox;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTestDelegate);

UCLASS()
class STONEAGECOLONY_API USpecifyCraftListItem : public USurvivalWidget
{
	GENERATED_BODY()
	
public:
	virtual void InitialSetup() override;

	UFUNCTION(BlueprintCallable)
	void OnSliderValueChanged();

	UFUNCTION(BlueprintCallable)
	void OnItemAmountBlockValueChanged();

	int32 CalculateMaxCraftAmount();
	int32 CalculateCraftAmountFromSlider();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Slider")
	USlider* Slider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "ItemNameBlock")
	UTextBlock* ItemNameBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "ItemAmountBlock")
	UTextBlock* ItemAmountBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "EditableItemAmountBlock")
	UEditableTextBox* EditableItemAmountBlock;

	int32 RepresentedItemID;
	AUsableActor* RepresentedItemInstance;
	int32 CraftAmount;
	UUISpecifyCrafting* OwnerSpecifyCraftingMenu;
	ASettlementMember* OwnerMember;
	UInventory* OwnerInventory;
};
