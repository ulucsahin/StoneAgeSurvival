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


EBTNodeResult::Type UBTTask_MoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	AEnemyAI *CharPC = Cast<AEnemyAI>(OwnerComp.GetAIOwner());
	APawn *playerCharacter = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(CharPC->EnemyKeyID));
	//CharPC->EnemyKeyID;

	if (playerCharacter) {
		//GLog->Log("lol enemy is not null");
		//Utility.RandomFloat(-250.f, 250.f);
		//CharPC->MoveToActor(playerCharacter, 5.f, true, true, true, 0, true);
		
		Act(CharPC);
		return EBTNodeResult::Succeeded;
	}
	else {
		return EBTNodeResult::Failed;
	}
	
	return EBTNodeResult::Failed;
}

void UBTTask_MoveToPlayer::Act(AEnemyAI* CharPC) {

	bool debug = false;

	//0: Idle, 1:Waiting, 2:Paused, 3:Moving
	if (CharPC->GetMoveStatus() == 0) {
		//FVector CurrentLoc = CharPC->GetActorLocation();
		if(debug)
			GLog->Log("Idle");
		//GetActorRotation
		
		FVector TargetLocation = FVector(132510.f + Utility.RandomFloat(-2500.f, 2500.f), 68390.f + Utility.RandomFloat(-2500.f, 2500.f), -8082.f + Utility.RandomFloat(-2500.f, 2500.f));
		CharPC->MoveToLocation(TargetLocation, 10.0f, true, true, true);
		//ai->MoveToLoc(this, FVector(132970.0f, 67990.0f, -8110.0f), 10.0f, true, true, true);
	}
	else if (CharPC->GetMoveStatus() == 1) {
		if(debug)
			GLog->Log("Waiting");
	}
	else if (CharPC->GetMoveStatus() == 2) {
		if(debug)
			GLog->Log("Paused");
	}
	else if (CharPC->GetMoveStatus() == 3) {
		if(debug)
			GLog->Log("Moving");
	}
}