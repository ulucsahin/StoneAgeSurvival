// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettlementMemberAI.h"
#include "UnoccupiedAI.generated.h"

/**
 * 
 */
UCLASS()
class STONEAGECOLONY_API AUnoccupiedAI : public ASettlementMemberAI
{
	GENERATED_BODY()
	
public:
	AUnoccupiedAI();

	virtual void Possess(APawn *InPawn) override;

	virtual void Act() override;

	virtual void CheckStatus() override;

	void WanderAround();
};
