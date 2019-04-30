// Fill out your copyright notice in the Description page of Project Settings.

#include "TestGameLoader.h"
#include "GameSaver.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void ATestGameLoader::OnUsed(APawn* InstigatorPawn) {
	UE_LOG(LogTemp, Warning, TEXT("TestGameLoader::OnUsed --- works"));


	// LOAD SYSTEM
	UGameSaver* GameLoader = Cast<UGameSaver>(UGameplayStatics::CreateSaveGameObject(UGameSaver::StaticClass()));
	GameLoader = Cast<UGameSaver>(UGameplayStatics::LoadGameFromSlot(GameLoader->SaveSlotName, GameLoader->UserIndex));
	//USurvivalGameInstance* GameInstance = GameLoader->GameInstance;

	/*if (GameInstance) {
		UE_LOG(LogTemp, Warning, TEXT("ObjectBed::OnUsed --- Loaded GameInstance available"));
	}*/

	if (GameLoader) {
		InstigatorPawn->SetActorLocation(GameLoader->PlayerLocation);
	}
}