// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SurvivalGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class STONEAGECOLONY_API USurvivalGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	FVector PlayerLocation;


};
