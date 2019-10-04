// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MyUtility.h"
//#include "EnemyAI.h"
#include "BTTask_MoveToPlayer.generated.h"

class ASettlementMemberAI;
//class MyUtility;
/**
 * 
 */
UCLASS()
class STONEAGECOLONY_API UBTTask_MoveToPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void Act(ASettlementMemberAI*);
	MyUtility Utility;

private:
	FTimerHandle UnusedHandle;
};
