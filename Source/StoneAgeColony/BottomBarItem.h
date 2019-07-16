// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIItemSlot.h"
#include "BottomBarItem.generated.h"

class UUIBottomBar;

/**
 * 
 */
UCLASS()
class STONEAGECOLONY_API UBottomBarItem : public UUIItemSlot //UUserWidget
{
	GENERATED_BODY()

public:
	//UBottomBarItem(const FObjectInitializer& ObjectInitializer);

	int32 IndexInBottomBar;

	bool Selected = false;

	void SetupBarItem(UUIBottomBar* BottomBar, int32 Index);

	UFUNCTION(BlueprintCallable, Category = "BarNotify")
	void NotifyBottomBar();

	UUIBottomBar* BottomBar;
};
