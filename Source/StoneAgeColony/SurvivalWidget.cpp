// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWidget.h"
#include "Runtime/UMG/Public/Components/Button.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "StoneAgeColonyCharacter.h"

void USurvivalWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USurvivalWidget::InitialSetup()
{

}


void USurvivalWidget::CloseMenu()
{
	IsActive = false;
	RemoveFromParent();

	auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	auto Player = (AStoneAgeColonyCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	//Player->OpenedMenus.Remove(this);
	int32 Index = Player->OpenedMenus.IndexOfByKey(this);
	Player->OpenedMenus[Index] = nullptr;

	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
		PlayerController->bEnableClickEvents = false;
		PlayerController->bEnableMouseOverEvents = false;
		
	}
}