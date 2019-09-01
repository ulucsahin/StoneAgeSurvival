// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Blueprint/UserWidget.h"
//#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "UIItemSlot.h"
//#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "UIInventoryItem.generated.h"

class UTextBlock;

UCLASS()
class STONEAGECOLONY_API UUIInventoryItem : public  UUIItemSlot //UUserWidget
{
	GENERATED_BODY()
	

public:
	UUIInventoryItem(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Stats")
	int IndexInInventory = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Stats")
	UTextBlock* ItemIDTextBlock;

	void OnClick() override; 	

	void SetupInventoryItemCell() override;

	UFUNCTION(BlueprintCallable, Category = "SubMenuButtons")
	void Use();
	
	UFUNCTION(BlueprintCallable, Category = "SubMenuButtons")
	void PlaceItem();

};
