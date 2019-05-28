// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjectBed.h"
#include "SaveGameEntity.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "StoneAgeColonyCharacter.h"
#include "Communicator.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectIterator.h"
#include "GameSaver.h"

AObjectBed::AObjectBed(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void AObjectBed::BeginPlay() 
{
	Super::BeginPlay();
}

void AObjectBed::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
}

void AObjectBed::OnUsed(APawn* InstigatorPawn) 
{
	GLog->Log("Bed used.");
	
	GameSaver GameSaverInstance;
	GameSaverInstance.SaveGame(InstigatorPawn);
}

int AObjectBed::GetID()
{
	return ID;
}

template <typename T>
void AObjectBed::IterateActors()
{	
	// We will reset spawned character details and update it with current details.
	Communicator::GetInstance().SpawnedCharacterDetails.Empty();
	UWorld* YourGameWorld = Communicator::GetInstance().World;

	for (TObjectIterator<T> Itr; Itr; ++Itr)
	{
		// Filter out objects not contained in the target world.
		if (Itr->GetWorld() != YourGameWorld)
		{
			continue;
		}
		if (std::is_same_v<T, AEnemyCharacter>) 
		{
			// Only do this if Destroy() on this object is not called.
			if (!Itr->IsPendingKill()) 
			{
				// Register details to communicator.
				Itr->RegisterActorDetailsToSave();
			}
		}
		
	}
}