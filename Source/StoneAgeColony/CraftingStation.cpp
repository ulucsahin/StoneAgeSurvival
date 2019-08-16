// Fill out your copyright notice in the Description page of Project Settings.

#include "CraftingStation.h"

void ACraftingStation::OnUsed(APawn* InstigatorPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("Im a crafting station"));
}

void ACraftingStation::SetupType(FString Type)
{

}