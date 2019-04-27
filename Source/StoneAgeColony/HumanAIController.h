// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include "CoreMinimal.h"
#include "AIController.h"

#include "HumanAIController.generated.h"


/**
 * 
 */
UCLASS()
class STONEAGECOLONY_API AHumanAIController : public AAIController
{
	GENERATED_BODY()
public:
	// Constructor and functions
	AHumanAIController();
	void test();
	//void MoveToLoc(FVector, float, bool, bool, bool);
	//
	// Variables
	//std::vector<int> characterIDList;

};
