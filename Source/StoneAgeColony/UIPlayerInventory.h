// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
//#include "Runtime/UMG/Public/Components/WrapBox.h"
//#include "UIInventoryItem.h"
//#include "StoneAgeColonyCharacter.h"
#include "UIPlayerInventory.generated.h"

class UWrapBox;
class UUIInventoryItem;
class AStoneAgeColonyCharacter;
/**
 * 
 */
UCLASS()
class STONEAGECOLONY_API UUIPlayerInventory : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUIPlayerInventory(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Lol")
	void RegisterToPlayer(AStoneAgeColonyCharacter* Player);

	UFUNCTION(BlueprintCallable, Category = "Lol")
	void AddItems();

	void Refresh();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Stats")
	int test;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Slots")
	UWrapBox* WrapBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Player")
	AStoneAgeColonyCharacter* Player;

	void Test();

	TSubclassOf<UUIInventoryItem> InvItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Tooltip")
	UUserWidget* ItemTooltipWidget;
};
