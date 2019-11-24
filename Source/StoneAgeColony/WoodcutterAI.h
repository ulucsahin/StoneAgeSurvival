// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettlementMemberAI.h"
#include "WoodcutterAI.generated.h"

/**
 * 
 */
UCLASS()
class STONEAGECOLONY_API AWoodcutterAI : public ASettlementMemberAI
{
	GENERATED_BODY()
	
public:
	AWoodcutterAI();
	virtual void Act() override;
	virtual void Possess(APawn* InstigatorPawn) override;
	virtual void CheckStatus() override;
};
