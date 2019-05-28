// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "UIInventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class STONEAGECOLONY_API UUIInventoryItem : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UUIInventoryItem(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Stats")
	int IndexInInventory = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Stats")
	int ItemID = 123;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Stats")
	UTextBlock* ItemIDTextBlock;

	UFUNCTION(BlueprintCallable, Category = "Lol")
	void SetupInventoryItemCell(UPARAM(ref) const int& index);
};
