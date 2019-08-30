// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/WrapBox.h"
#include "Runtime/UMG/Public/Components/VerticalBox.h"
#include "StoneAgeColonyCharacter.h"
#include "UIItemSlot.h"
#include "CraftingStationMenu.generated.h"

class UCraftListButton;


UCLASS()
class STONEAGECOLONY_API UCraftingStationMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UCraftingStationMenu(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "CraftingItems")
	UVerticalBox* VerticalBox;

	UFUNCTION(BlueprintCallable, Category = "Lol")
	void RegisterToPlayer(AStoneAgeColonyCharacter* Player);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Player")
	AStoneAgeColonyCharacter* Player;

	UFUNCTION(BlueprintCallable, Category = "Lol")
	void AddItems(int32 CraftStationID);

	TSubclassOf<UCraftListButton> CraftMenuItem;
};
