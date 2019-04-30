// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjectBed.h"
#include "GameSaver.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "StoneAgeColonyCharacter.h"
#include "SurvivalGameInstance.h"


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
	GLog->Log("bed used");

	/*UGameSaver* SaveGameInstance = Cast<UGameSaver>(UGameplayStatics::CreateSaveGameObject(UGameSaver::StaticClass()));
	SaveGameInstance->PlayerName = "uluc12313131";
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, SaveGameInstance->UserIndex);
	
	UGameSaver* LoadGameInstance = Cast<UGameSaver>(UGameplayStatics::CreateSaveGameObject(UGameSaver::StaticClass()));
	LoadGameInstance = Cast<UGameSaver>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, LoadGameInstance->UserIndex));
	FString PlayerNameToDisplay = LoadGameInstance->PlayerName;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, PlayerNameToDisplay);
	}*/


	// SAVE SYSTEM WORKING
	USurvivalGameInstance* GameInstance = Cast<USurvivalGameInstance>(GetGameInstance());
	UGameSaver* GameSaver = Cast<UGameSaver>(UGameplayStatics::CreateSaveGameObject(UGameSaver::StaticClass()));
	//GameSaver->GameInstance = GameInstance;
	GameSaver->PlayerLocation = InstigatorPawn->GetActorLocation();
	UGameplayStatics::SaveGameToSlot(GameSaver, GameSaver->SaveSlotName, GameSaver->UserIndex);
	
}