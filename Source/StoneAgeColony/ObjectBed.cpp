// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjectBed.h"
#include "GameSaver.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "StoneAgeColonyCharacter.h"
#include "SurvivalGameInstance.h"
#include "Communicator.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectIterator.h"

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
	
	// Test
	Communicator::GetInstance().Increment();

	// Save System
	UGameSaver* GameSaver = Cast<UGameSaver>(UGameplayStatics::CreateSaveGameObject(UGameSaver::StaticClass()));
	Communicator::GetInstance().PlayerLocation = InstigatorPawn->GetActorLocation();


	// Iterate over actors in world to get details of spawned actors.
	IterateActors<AEnemyCharacter>();

	// Assign variables to save file (from communicator).
	GameSaver->PlayerLocation = Communicator::GetInstance().PlayerLocation;
	GameSaver->test = Communicator::GetInstance().test;
	GameSaver->SpawnedCharacterDetails = Communicator::GetInstance().SpawnedCharacterDetails;

	//UE_LOG(LogTemp, Warning, TEXT("First location in communicator: %f"), GameSaver->SpawnedCharacterDetails[0].CharacterLocation.X);
	// Save save-file to disk.
	UGameplayStatics::SaveGameToSlot(GameSaver, GameSaver->SaveSlotName, GameSaver->UserIndex);
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