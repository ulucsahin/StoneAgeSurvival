// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettlementMemberAI.h"
#include "StationWorkerAI.h"
#include "FarmerAI.generated.h"


class APlant;

UCLASS()
class STONEAGECOLONY_API AFarmerAI : public AStationWorkerAI
{
	GENERATED_BODY()
	
public:
	AFarmerAI();
	virtual void Act() override;
	virtual void Possess(APawn* InstigatorPawn) override;
	virtual void CheckStatus() override;
	APlant* StartPlanting();
	FName SelectSocketToPlant();
	void HarvestGrownPlants();

	float ActivityTimer = 5.f;
	
};
