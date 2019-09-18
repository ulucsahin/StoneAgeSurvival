// Fill out your copyright notice in the Description page of Project Settings.

#include "Structure.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "StoneAgeColonyCharacter.h"


// Called when the game starts or when spawned
void AStructure::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStructure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStructure::OpenMenu(APawn* InstigatorPawn)
{

}

void AStructure::OnUsed(APawn* InstigatorPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("Im a structure"));
}

void AStructure::EmptyCommunicatorDetailsArray()
{
	UE_LOG(LogTemp, Warning, TEXT("AStructure::EmptyCommunicatorDetailsArray"));
	// to be implemented
}

void AStructure::RegisterActorDetailsToSave()
{
	UE_LOG(LogTemp, Warning, TEXT("AStructure::RegisterActorDetailsToSave"));
	// to be implemented
}

void AStructure::SpawnLoadedActors()
{
	UE_LOG(LogTemp, Warning, TEXT("AStructure::SpawnLoadedActors"));
	// to be implemented
}