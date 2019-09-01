// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
//#include "GameLoadManager.h"
#include "UIMainMenu.generated.h"

class AGameLoadManager;

/**
 * 
 */
UCLASS()
class STONEAGECOLONY_API UUIMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UUIMainMenu(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = true), Category = "Slots")
	AGameLoadManager* Loader;// = NewObject<AGameLoadManager>();

	UFUNCTION(BlueprintCallable, Category = "Initialize")
	void InitializeVariables();

};
