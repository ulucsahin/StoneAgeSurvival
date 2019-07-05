// Fill out your copyright notice in the Description page of Project Settings.

#include "GameLoader.h"
#include "SaveGameEntity.h"
#include "PeopleSpawner.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Communicator.h"
#include "EnemyCharacter.h"
#include "GatherableTree.h"
#include "Building.h"
#include "PeopleSpawner.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "StoneAgeColonyCharacter.h"
#include "SurvivalGameState.h"

GameLoader::GameLoader()
{
}

GameLoader::~GameLoader()
{
}

void GameLoader::LoadGame(APawn* InstigatorPawn)
{
	/* This method handles everything about loading game from a savefile. */

	// LOAD SYSTEM
	USaveGameEntity* SaveGameEntityLoad = Cast<USaveGameEntity>(UGameplayStatics::CreateSaveGameObject(USaveGameEntity::StaticClass()));
	SaveGameEntityLoad = Cast<USaveGameEntity>(UGameplayStatics::LoadGameFromSlot(SaveGameEntityLoad->SaveSlotName, SaveGameEntityLoad->UserIndex));

	if (SaveGameEntityLoad) {
		// Destroy existing characters that should be deleted before loading.
		DestroyActors<AEnemyCharacter>();
		DestroyActors<AGatherableTree>();
		DestroyActors<ABuilding>();

		// Load varibles to communicator (update with loaded variables).
		Communicator::GetInstance().test = SaveGameEntityLoad->test;
		Communicator::GetInstance().SpawnedCharacterDetails = SaveGameEntityLoad->SpawnedCharacterDetails;
		Communicator::GetInstance().SpawnedGatherableTreeDetails = SaveGameEntityLoad->SpawnedGatherableTreeDetails;
		Communicator::GetInstance().SpawnedBuildingDetails = SaveGameEntityLoad->SpawnedBuildingDetails;
		Communicator::GetInstance().PlayerTransform = SaveGameEntityLoad->PlayerTransform;
		Communicator::GetInstance().PlayerRotation = SaveGameEntityLoad->PlayerRotation;
		Communicator::GetInstance().PlayerHealth = SaveGameEntityLoad->PlayerHealth;
		Communicator::GetInstance().PlayerLevel = SaveGameEntityLoad->PlayerLevel;
		Communicator::GetInstance().PlayerExperience = SaveGameEntityLoad->PlayerExperience;
		Communicator::GetInstance().PlayerGold = SaveGameEntityLoad->PlayerGold;
		Communicator::GetInstance().ElapsedGameMinutes = SaveGameEntityLoad->ElapsedGameMinutes;

		// Load player variables.
		((AStoneAgeColonyCharacter*)InstigatorPawn)->SetActorTransform(SaveGameEntityLoad->PlayerTransform);
		((AStoneAgeColonyCharacter*)InstigatorPawn)->SetActorRotation(SaveGameEntityLoad->PlayerRotation);
		((AStoneAgeColonyCharacter*)InstigatorPawn)->Health = Communicator::GetInstance().PlayerHealth;
		((AStoneAgeColonyCharacter*)InstigatorPawn)->Level = Communicator::GetInstance().PlayerLevel;
		((AStoneAgeColonyCharacter*)InstigatorPawn)->Experience = Communicator::GetInstance().PlayerExperience;
		((AStoneAgeColonyCharacter*)InstigatorPawn)->Gold = Communicator::GetInstance().PlayerGold;
		((AStoneAgeColonyCharacter*)InstigatorPawn)->Inventory = SaveGameEntityLoad->PlayerInventory;

		ASurvivalGameState* CurrentGameState = Cast<ASurvivalGameState>(Communicator::GetInstance().World->GetGameState());
		CurrentGameState->ElapsedGameMinutes = Communicator::GetInstance().ElapsedGameMinutes;

		// Update UI Inventory Elements
		UpdateInventoryUI();

		// Spawn saved characters.
		SpawnLoadedActors<AEnemyCharacter>();
		SpawnLoadedActors<AGatherableTree>();
		SpawnLoadedActors<ABuilding>();
	}

	UE_LOG(LogTemp, Warning, TEXT("GameLoader: Game loaded."));
}

template <typename T>
void GameLoader::SpawnLoadedActors() 
{
	/* Spawn previously saved characters from savefile. */
	T::SpawnLoadedActors();
}

template <typename T>
void GameLoader::DestroyActors() 
{
	/* Actors are needed to be destroyed first otherwise we would have duplicates. */

	// We will reset spawned character details and update it with current details.
	UWorld* YourGameWorld = Communicator::GetInstance().World;

	for (TObjectIterator<T> Itr; Itr; ++Itr)
	{
		// Filter out objects not contained in the target world.
		if (Itr->GetWorld() != YourGameWorld)
		{
			continue;
		}
		else
		{
			Itr->Destroy();
		}
		
	}

	// Empty communicator since we deleted all characters.
	T::EmptyCommunicatorDetailsArray();
}

void GameLoader::UpdateInventoryUI()
{
	//FStringClassReference HUDRef(TEXT("/Game/Uluc/HUD/BP_Widget.BP_Widget_C"));
	//if (UClass* HUD = MyWidgetClassRef.TryLoadClass<UUserWidget>())
	//{
	//	UUserWidget* MyWidget = CreateWidget<UUserWidget>(this, HUD);
	//	// Do stuff with MyWidget
	//	HUD->AddToViewport(9999);
	//}

	//UFloatProperty* FloatProp = FindField(Object->GetClass(), PropertyName);
	//if (FloatProp != NULL)
	//{
	//	float* FloatPtr = FloatProp->GetPropertyValue_InContainer(Object);
	//	if (FloatPtr != NULL)
	//	{
	//		float MyFloat = *FloatPtr;
	//	}
	//}


}