// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIPlayerInventory.generated.h"

/**
 * 
 */
UCLASS()
class STONEAGECOLONY_API UUIPlayerInventory : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Stats")
	int test;

	void Test();
};
