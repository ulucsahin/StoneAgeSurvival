// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SurvivalWidget.h"
#include "UIPlayerInventory.generated.h"

class UWrapBox;
class UUIInventoryItem;
class AStoneAgeColonyCharacter;
//class ASettlementMember;
class AHumanCharacter;
/**
 * 
 */
UCLASS()
class STONEAGECOLONY_API UUIPlayerInventory : public USurvivalWidget
{
	GENERATED_BODY()
	
public:
	UUIPlayerInventory(const FObjectInitializer& ObjectInitializer);

	void InitialSetup(AHumanCharacter* Owner);

	UFUNCTION(BlueprintCallable, Category = "Lol")
	void RegisterToPlayer();

	UFUNCTION(BlueprintCallable, Category = "Lol")
	void SetOwner(AHumanCharacter* Owner);

	UFUNCTION(BlueprintCallable, Category = "Lol")
	void AddItems();

	void Refresh();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Stats")
	int test;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Slots")
	UWrapBox* WrapBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Player")
	AStoneAgeColonyCharacter* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "SettlementMember")
	AHumanCharacter* Owner; // this class can show NPC's inventory too. this variable used if so.

	TSubclassOf<UUIInventoryItem> InvItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Tooltip")
	UUserWidget* ItemTooltipWidget;
};
