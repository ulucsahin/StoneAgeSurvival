// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StoneAgeColonyCharacter.h"
#include "Blueprint/UserWidget.h"
#include "UIItemSlot.generated.h"

/** Base class for all slot objects in user interface such as items in inventory etc.
 * 
 */
UCLASS()
class STONEAGECOLONY_API UUIItemSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUIItemSlot(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Stats")
	int32 ItemID = 123;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Stats")
	int32 ItemAmount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Stats")
	FString ItemName = "PlaceholderItemName";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Stats")
	UTexture2D* InventoryTexture;

	UFUNCTION(BlueprintCallable, Category = "Lol")
	virtual void SetupInventoryItemCell(); 	//void SetupInventoryItemCell(UPARAM(ref) const int& id);

	UFUNCTION(BlueprintCallable, Category = "MouseEvents")
	virtual void OnClick();

	AStoneAgeColonyCharacter* PlayerCharacter;

};
