// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettlementMemberAI.h"
#include "StationWorkerAI.h"
#include "FarmerAI.generated.h"

UCLASS()
class STONEAGECOLONY_API AFarmerAI : public AStationWorkerAI
{
	GENERATED_BODY()
	
public:
	AFarmerAI();
	virtual void Act() override;
	virtual void Possess(APawn* InstigatorPawn) override;
	virtual void CheckStatus() override;
	void StartPlanting();
	FName SelectSocketToPlant();
	
};
