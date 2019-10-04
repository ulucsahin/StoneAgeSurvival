// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MoveToPlayer.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "StoneAgeColonyCharacter.h"
#include "EnemyCharacter.h"
#include "MyUtility.h"
#include "TimerManager.h"
#include "SettlementMemberAI.h"
#include "MyUtility.h"


EBTNodeResult::Type UBTTask_MoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
{
	//UE_LOG(LogTemp, Warning,TEXT("UBTTask_MoveToPlayer:: ExecuteTask"));
	ASettlementMemberAI *CharPC = Cast<ASettlementMemberAI>(OwnerComp.GetAIOwner());


	APawn *playerCharacter = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(CharPC->EnemyKeyID));
	//CharPC->EnemyKeyID;
	Act(CharPC);
	//UE_LOG(LogTemp, Warning, TEXT("UBTTask_MoveToPlayer::ExecuteTask2"));
	if (playerCharacter) 
	{
		//UE_LOG(LogTemp, Warning, TEXT("UBTTask_MoveToPlayer:: if (playerCharacter) "));
		
		Act(CharPC);
		return EBTNodeResult::Succeeded;
	}
	else 
	{
		//UE_LOG(LogTemp, Warning, TEXT("UBTTask_MoveToPlayer:: else "));
		return EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::Failed;
}

void UBTTask_MoveToPlayer::Act(ASettlementMemberAI* CharPC)
{
	//0: Idle, 1:Waiting, 2:Paused, 3:Moving
	if (CharPC->GetMoveStatus() == 0) 
	{

		GLog->Log("Idle");

		FVector TargetLocation = FVector(132510.f + Utility.RandomFloat(-2500.f, 2500.f), 68390.f + Utility.RandomFloat(-2500.f, 2500.f), -8082.f + Utility.RandomFloat(-2500.f, 2500.f));
		CharPC->MoveToLocation(TargetLocation, 10.0f, true, true, true);
	}
	else if (CharPC->GetMoveStatus() == 1) 
	{

		GLog->Log("Waiting");
	}
	else if (CharPC->GetMoveStatus() == 2) 
	{

		GLog->Log("Paused");
	}
	else if (CharPC->GetMoveStatus() == 3) 
	{

		GLog->Log("Moving");
	}
}