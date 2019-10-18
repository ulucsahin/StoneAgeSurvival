// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettlementMemberAI.h"
#include "MorningRegularAI.generated.h"

/**
 * 
 */
UCLASS()
class STONEAGECOLONY_API AMorningRegularAI : public ASettlementMemberAI
{
	GENERATED_BODY()
	
public:
	AMorningRegularAI();

	virtual void Act() override;

	void BreakfastTogether();
};
