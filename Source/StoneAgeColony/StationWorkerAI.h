// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettlementMemberAI.h"
#include "StationWorkerAI.generated.h"

class ACraftingStation;

UCLASS()
class STONEAGECOLONY_API AStationWorkerAI : public ASettlementMemberAI
{
	GENERATED_BODY()
public:
	AStationWorkerAI();

	virtual void Act() override;
	virtual void Possess(APawn* InstigatorPawn) override;
	virtual void CheckStatus() override;
	void MoveToStation();
	void StartCrafting();
	float GetDistanceToWorkstation();
	

private:
	ACraftingStation* WorkStation;
	FTimerHandle TimerHandle;
};
