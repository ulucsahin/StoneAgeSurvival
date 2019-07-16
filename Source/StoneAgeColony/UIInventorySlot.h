// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIItemSlot.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "UIInventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class STONEAGECOLONY_API UUIInventorySlot : public UUIItemSlot
{
	GENERATED_BODY()
	
public:
	UUIInventorySlot(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Stats")
	int IndexInInventory = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Stats")
	UTextBlock* ItemIDTextBlock;

	//UFUNCTION(BlueprintCallable, Category = "Lol")
	void SetupInventoryItemCell() override; 	//void SetupInventoryItemCell(UPARAM(ref) const int& id);

	//UFUNCTION(BlueprintCallable, Category = "MouseEvents")
	//void OnClick();
};
