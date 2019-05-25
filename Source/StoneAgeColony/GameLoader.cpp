// Fill out your copyright notice in the Description page of Project Settings.

#include "GameLoader.h"
#include "SaveGameEntity.h"
#include "PeopleSpawner.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Communicator.h"
#include "EnemyCharacter.h"
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
	/*
		This method handles everything about loading game from a savefile.
	*/

	// LOAD SYSTEM
	USaveGameEntity* SaveGameEntityLoad = Cast<USaveGameEntity>(UGameplayStatics::CreateSaveGameObject(USaveGameEntity::StaticClass()));
	SaveGameEntityLoad = Cast<USaveGameEntity>(UGameplayStatics::LoadGameFromSlot(SaveGameEntityLoad->SaveSlotName, SaveGameEntityLoad->UserIndex));

	if (SaveGameEntityLoad) {
		// Destroy existing characters.
		DestroyActors<AEnemyCharacter>();

		// Set varibles to communicator (update with loaded variables).
		Communicator::GetInstance().test = SaveGameEntityLoad->test;
		Communicator::GetInstance().SpawnedCharacterDetails = SaveGameEntityLoad->SpawnedCharacterDetails;
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
		SpawnCharacters();
		UE_LOG(LogTemp, Warning, TEXT("XDXDXDXD."));
	}

	UE_LOG(LogTemp, Warning, TEXT("GameLoader: Game loaded."));
}

void GameLoader::SpawnCharacters() 
{
	/*
	Spawn previously saved characters from savefile.
	*/

	FActorSpawnParameters SpawnParams;
	//SpawnParams.Owner = this;

	// Get actor details to spawn from communicator.
	TArray<FEnemyCharacterDetails> ActorDetailsToSpawn = Communicator::GetInstance().SpawnedCharacterDetails;
	auto ActorToSpawn = Communicator::GetInstance().EnemyCharacterToSpawn;

	UE_LOG(LogTemp, Warning, TEXT("ActorDetailsToSpawn lenght: %d"), ActorDetailsToSpawn.Num());
	// Iterate over array and saved spawn actors.
	for (FEnemyCharacterDetails Details : ActorDetailsToSpawn)
	{
		FTransform ActorTransform = Details.CharacterTransform;
		AEnemyCharacter* SpawnedActor = Communicator::GetInstance().World->SpawnActor<AEnemyCharacter>(ActorToSpawn, ActorTransform, SpawnParams);
	}

}

template <typename T>
void GameLoader::DestroyActors() 
{
	/*
	Actors are needed to be destroyed first otherwise we would have duplicates.
	This method fixes this problem.
	*/

	// We will reset spawned character details and update it with current details.
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
			// Register details to communicator.
			Itr->Destroy();
		}

	}

	// Empty communicator since we deleted all characters.
	Communicator::GetInstance().SpawnedCharacterDetails.Empty();
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