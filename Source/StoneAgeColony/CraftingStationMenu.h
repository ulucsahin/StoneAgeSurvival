// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Blueprint/UserWidget.h"
#include "SurvivalWidget.h"
#include "Runtime/Engine/Classes/Components/TimelineComponent.h"
#include "CraftingStationMenu.generated.h"

class AUsableActor;
class UCraftListButton;
class AStoneAgeColonyCharacter;
class UTextBlock;
class UVerticalBox;
class UProgressBar;
class UTimelineComponent;
class UCurveFloat;
//class FOnTimelineFloat;
//class FOnTimelineEvent;

UCLASS()
class STONEAGECOLONY_API UCraftingStationMenu : public USurvivalWidget
{
	GENERATED_BODY()
	
public:
	UCraftingStationMenu(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "CraftingItems")
	UVerticalBox* VerticalBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "CraftingItems")
	UTextBlock* DescriptionText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "CraftingItems")
	UTextBlock* RequirementsText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "CraftingItems")
	UProgressBar* CraftingProgressBar;

	UFUNCTION(BlueprintCallable, Category = "Lol")
	void RegisterToPlayer(AStoneAgeColonyCharacter* Player);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Player")
	AStoneAgeColonyCharacter* Player;

	UFUNCTION(BlueprintCallable, Category = "Lol")
	void AddItems(int32 CraftStationID);

	void ReceiveInformationFromButton(AUsableActor* RepresentedItem, int32 ItemID);

	bool CraftingRequirementsMet();

	void StartCrafting(float CraftingTime);

	UFUNCTION()
	void StartUpdatingProgressBar(float CraftingTime);

	UFUNCTION() // ufunction needed for timer
	void UpdateProgressBar(float CraftingTime, float UpdateFrequency);

	UFUNCTION(BlueprintCallable, Category = "Lol") 
	void CloseMenu();

	AUsableActor* CurrentItem;
	int32 CurrentItemID;
	int32 CraftAmount;
	TSubclassOf<UCraftListButton> CraftMenuItem;
	UTimelineComponent* CraftProgressTimeline;
	FTimerHandle TimerHandle;
	bool CurrentlyCrafting;
	float CraftingBarProgress;
};
