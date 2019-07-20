// Fill out your copyright notice in the Description page of Project Settings.

#include "TestGameLoader.h"
#include "GameLoadManager.h"
#include "PeopleSpawner.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Communicator.h"
#include "EnemyCharacter.h"


void ATestGameLoader::OnUsed(APawn* InstigatorPawn)
{
	/*
	This method calls GameLoader class to handle loading system.
	*/
	UE_LOG(LogTemp, Warning, TEXT("ATestGameLoader::OnUsed"));
	if (InstigatorPawn)
	{
		AGameLoadManager* GameLoaderInstance = NewObject<AGameLoadManager>();
		GameLoaderInstance->LoadGame(InstigatorPawn);
	}
	

}

int ATestGameLoader::GetID()
{
	return ID;
}