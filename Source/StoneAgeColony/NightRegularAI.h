// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettlementMemberAI.h"
#include "NightRegularAI.generated.h"

/**
 * 
 */
UCLASS()
class STONEAGECOLONY_API ANightRegularAI : public ASettlementMemberAI
{
	GENERATED_BODY()

public:
	ANightRegularAI();
	virtual void Act() override;
	void GoHome();
};
