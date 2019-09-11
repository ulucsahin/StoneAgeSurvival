// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structure.h"
#include "House.generated.h"

/**
 * 
 */
UCLASS()
class STONEAGECOLONY_API AHouse : public AStructure
{
	GENERATED_BODY()
	
public:
	void SetupType(FString);

private:
	FName HouseType;
};
