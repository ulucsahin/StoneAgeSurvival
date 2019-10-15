// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettlementMemberAI.h"
#include "StationWorkerAI.generated.h"

/**
 * 
 */
UCLASS()
class STONEAGECOLONY_API AStationWorkerAI : public ASettlementMemberAI
{
	GENERATED_BODY()
public:
	AStationWorkerAI();

	virtual void Act() override;

	void MoveToStation();
};
