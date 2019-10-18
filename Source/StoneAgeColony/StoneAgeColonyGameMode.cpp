// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.


#include "StoneAgeColonyGameMode.h"
#include "StoneAgeColony.h"
#include "StoneAgeColonyHUD.h"
#include "StoneAgeColonyCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "SurvivalGameState.h"
#include "SurvivalGameInstance.h"
#include "Communicator.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectIterator.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectBaseUtility.h"

#include "ObjectFactory.h"

// Usable Actors
#include "UsableActor.h"
#include "TestGameLoader.h"
#include "ObjectBed.h"
#include "PeopleSpawner.h"

// Gatherable Actors
#include "GatherableTree.h"

// Other Actors
#include "Building.h"
#include "Edible.h"

// HUD
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "SettlementMember.h"


AStoneAgeColonyGameMode::AStoneAgeColonyGameMode(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	// Set world for communicator
	Communicator::GetInstance().World = GetWorld();

	// Set communicator blueprints
	/*static ConstructorHelpers::FClassFinder<AEnemyCharacter> BPClass(TEXT("'/Game/Uluc/ActiveAssets/BP_FollowerEnemyCharacter'"));
	Communicator::GetInstance().EnemyCharacterBlueprint = BPClass.Class;*/

	static ConstructorHelpers::FClassFinder<ASettlementMember> BPClass2(TEXT("'/Game/Uluc/NPC/BP_SettlementMember'"));
	Communicator::GetInstance().SettlementMemberBlueprint = BPClass2.Class;
	
	//static ConstructorHelpers::FClassFinder<AGatherableTree> BPClass2(TEXT("'/Game/Uluc/ActiveAssets/GatherableObjects/BP_GatherableTree'"));
	//Communicator::GetInstance().GatherableTreeBlueprint = BPClass2.Class;
	static ConstructorHelpers::FClassFinder<ABuilding> BPClass3(TEXT("/Game/Uluc/BuildingSystem/Blueprints/BP_Building"));
	Communicator::GetInstance().BuildingBlueprint = BPClass3.Class;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AStoneAgeColonyHUD::StaticClass();

	/* Assign the class types used by this gamemode */
	//PlayerControllerClass = ASPlayerController::StaticClass();
	//PlayerStateClass = ASPlayerState::StaticClass();
	GameStateClass = ASurvivalGameState::StaticClass();
	//SpectatorClass = ASSpectatorPawn::StaticClass();

	bAllowFriendlyFireDamage = false;
	bSpawnZombiesAtNight = true;

	/* Start the game at 06:00 */
	TimeOfDayStart = 6 * 60;
	BotSpawnInterval = 5.0f;

	/* Default team is 1 for players and 0 for enemies */
	PlayerTeamNum = 1;
	//RegisterItemIDs();
}


void AStoneAgeColonyGameMode::InitGameState()
{
	Super::InitGameState();

	// Set usable item IDs
	RegisterItemIDs();

	// Resets communicator variables.
	Communicator::GetInstance().Reset();
	//Communicator::GetInstance().SetupAssets();

	ASurvivalGameState* MyGameState = Cast<ASurvivalGameState>(GameState);
	if (MyGameState)
	{
		MyGameState->ElapsedGameMinutes = TimeOfDayStart;
	}
}

void AStoneAgeColonyGameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	/* Set timer to run every second */
	GetWorldTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &AStoneAgeColonyGameMode::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation(), true);
}


void AStoneAgeColonyGameMode::StartMatch() { 
	
}

void AStoneAgeColonyGameMode::DefaultTimer() {
	ASurvivalGameState* MyGameState = Cast<ASurvivalGameState>(GameState);
	if (MyGameState)
	{
		/* Increment our time of day */
		MyGameState->ElapsedGameMinutes += MyGameState->GetTimeOfDayIncrement();

		/* Register time passed */
		Communicator::GetInstance().ElapsedGameMinutes = MyGameState->ElapsedGameMinutes;

		/* Determine our state */
		MyGameState->GetAndUpdateIsNight();

		/* Update settlement member ai for morning night etc switches */
		MyGameState->UpdateNPCAI();

		/* Trigger events when night starts or ends */
		bool CurrentIsNight = MyGameState->GetIsNight();
		if (CurrentIsNight != LastIsNight)
		{
			/* The night just ended, respawn all dead players */
			if (!CurrentIsNight)
			{
				OnNightEnded();
			}
		}
	}
}

void AStoneAgeColonyGameMode::OnNightEnded() {

}

// Sets APlayerStart of our pawn on BeginPlay, using Game Instance variables
// https://youtu.be/2aUYBzmefpM?t=355
//AActor* AStoneAgeColonyGameMode::ChoosePlayerStart_Implementation(AController* Player) 
//{
//
//	class AActor* retVal = nullptr;
//	//Player->SetActorLocation(FVector(23030.f, -180.f, 90.f));
//	//if (Player)
//	//{
//	//	class UWorld* const world = GetWorld();
//
//	//	if (world != nullptr)
//	//	{
//	//		class USurvivalGameInstance* gInstance = Cast<USurvivalGameInstance>(GetGameInstance());
//
//	//		if (gInstance)
//	//		{
//	//			TArray<class AActor*> PlayerStarts;
//	//			UGameplayStatics::GetAllActorsOfClass(world, APlayerStart::StaticClass(), PlayerStarts);
//	//			TArray<class AActor*> PreferredStarts;
//
//	//			for (TActorIterator<APlayerStart> Itr(world); Itr; ++Itr) 
//	//			{
//	//				// NOT IMPLEMENTED
//	//				// add *Itr to PreferredStarts array according to some condition
//
//	//			}
//
//	//			return PreferredStarts[FMath::RandRange(0, PreferredStarts.Num() - 1)];
//	//		}
//
//	//	}
//
//	//}
//
//	return retVal;
//}
//
//bool AStoneAgeColonyGameMode::ShouldSpawnAtStartSpot(AController* Player)
//{
//	return true;
//}

void AStoneAgeColonyGameMode::RegisterItemIDs() 
{
	// this is retarded

	//AObjectFactory* Factory = NewObject<AObjectFactory>();

	//AUsableActor* test = NewObject<AUsableActor>();
	//Communicator::GetInstance().UsableItemIDMap.Add(  0/*AUsableActor::StaticClass()->GetDefaultObject<AUsableActor>()->ID*/,	   NewObject<AUsableActor>()); 
	//Communicator::GetInstance().UsableItemIDMap.Add(  ATestGameLoader::StaticClass()->GetDefaultObject<ATestGameLoader>()->ID,     NewObject<ATestGameLoader>()); 
	//Communicator::GetInstance().UsableItemIDMap.Add(  APeopleSpawner::StaticClass()->GetDefaultObject<APeopleSpawner>()->ID,	   NewObject<APeopleSpawner>()); 
	//Communicator::GetInstance().UsableItemIDMap.Add(  AObjectBed::StaticClass()->GetDefaultObject<AObjectBed>()->ID,			   NewObject<AObjectBed>()); 

	//auto tmp = Factory->CreateObject<AGatherableTree>(100); //NewObject<AGatherableTree>(); //tmp->SetupType("GatherableTree");
	//Communicator::GetInstance().UsableItemIDMap.Add(tmp->ID, tmp);

	//auto tmp2 = Factory->CreateObject<AEdible>(200);//NewObject<AEdible>(); //tmp2->SetupType("Apple");
	//Communicator::GetInstance().UsableItemIDMap.Add(tmp2->ID, tmp2);

	//ASettlement* tmp3 = NewObject<ASettlement>(); // Factory->CreateObject<ASettlement>(10000);//NewObject<AEdible>(); //tmp2->SetupType("Apple");
	//Communicator::GetInstance().UsableItemIDMap.Add(tmp3->ID, tmp3);

	//for (auto& item : Communicator::GetInstance().UsableItemIDMap)
	//{
	//	// Add objects in UsableItemIDMap to RootSet so they will not be garbage collected during gameplay.
	//	item.Value->AddToRoot();
	//}
}