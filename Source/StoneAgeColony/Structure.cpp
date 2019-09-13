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

void AStructure::OnUsed(APawn* InstigatorPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("Im a structure"));
}
