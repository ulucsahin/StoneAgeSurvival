// Fill out your copyright notice in the Description page of Project Settings.

#include "WoodcutterAI.h"
#include "Communicator.h"


AWoodcutterAI::AWoodcutterAI()
{
	
}

void AWoodcutterAI::Act()
{
	Super::Act();
}

void AWoodcutterAI::Possess(APawn* InstigatorPawn)
{
	Super::Possess(InstigatorPawn);
	Communicator::GetInstance().World->GetTimerManager().SetTimer(TimerHandle, this, &AWoodcutterAI::CheckStatus, 1.0f, true);
}

void AWoodcutterAI::CheckStatus()
{
	UE_LOG(LogTemp, Warning, TEXT("ima woodkutah"));
}