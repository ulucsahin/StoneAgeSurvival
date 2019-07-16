// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BottomBarItem.h"
#include "StoneAgeColonyCharacter.h"
#include "Runtime/UMG/Public/Components/WrapBox.h"
#include "UIBottomBar.generated.h"

/**
 * 
 */
UCLASS()
class STONEAGECOLONY_API UUIBottomBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UUIBottomBar(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Lol")
	void RegisterToPlayer(AStoneAgeColonyCharacter* Player);

	UFUNCTION(BlueprintCallable, Category = "Lol")
	void InitializeBottomBarItems();
	
	UFUNCTION(BlueprintCallable, Category = "Lol")
	void AddItems();

	UFUNCTION(BlueprintCallable, Category = "Lol")
	void SetItemAtIndex(UBottomBarItem* Item);

	void Refresh();
	void SelectSlot(int32 Index);
	void SelectNextSlot();
	void SelectPreviousSlot();
	void GetNotification(UBottomBarItem* Item);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Slots")
	TArray<int> Test;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Slots")
	UWrapBox* WrapBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Slots")
	TArray<UBottomBarItem*> BarItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Player")
	AStoneAgeColonyCharacter* Player;

	TSubclassOf<UBottomBarItem> InvItemClass;

	int32 SelectedSlot;
};
