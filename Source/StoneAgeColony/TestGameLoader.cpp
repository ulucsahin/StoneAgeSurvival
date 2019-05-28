// Fill out your copyright notice in the Description page of Project Settings.

#include "TestGameLoader.h"
#include "GameLoader.h"
#include "PeopleSpawner.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Communicator.h"
#include "EnemyCharacter.h"


void ATestGameLoader::OnUsed(APawn* InstigatorPawn)
{
	/*
	This method calls GameLoader class to handle loading system.
	*/
	GameLoader* GameLoaderInstance = new GameLoader();
	GameLoaderInstance->LoadGame(InstigatorPawn);

}

int ATestGameLoader::GetID()
{
	return ID;
}