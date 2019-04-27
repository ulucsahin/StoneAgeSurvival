// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAI.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include <string>
#include "EnemyCharacter.h"


AEnemyAI::AEnemyAI() {

	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
}

void AEnemyAI::Possess(APawn *InPawn) {
	Super::Possess(InPawn);

	AEnemyCharacter *Char = Cast<AEnemyCharacter>(InPawn);

	if (Char && Char->BotBehavior) {
		BlackboardComp->InitializeBlackboard(*Char->BotBehavior->BlackboardAsset);
		EnemyKeyID = BlackboardComp->GetKeyID("Target");
		//GLog->Log(EnemyKeyID);
		BehaviorComp->StartTree(*Char->BotBehavior);
	}

}