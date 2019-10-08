// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SettlementMemberAI.generated.h"

class AStructure;

UCLASS()
class STONEAGECOLONY_API ASettlementMemberAI : public AAIController
{
	GENERATED_BODY()
	
	UPROPERTY(transient)
	class UBlackboardComponent *BlackboardComp;

	UPROPERTY(transient)
	class UBehaviorTreeComponent *BehaviorComp;


public:

	ASettlementMemberAI();

	virtual void Possess(APawn *InPawn) override;
	void SetTargetEnemy(APawn* NewTarget);
	void SetTargetInRange(bool InRange);
	void MoveToWorkingStation();

	uint8 EnemyKeyID;

};
