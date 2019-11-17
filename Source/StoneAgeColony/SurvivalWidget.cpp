// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWidget.h"
#include "Runtime/UMG/Public/Components/Button.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "StoneAgeColonyCharacter.h"
#include "InterfaceManager.h"

void USurvivalWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USurvivalWidget::InitialSetup()
{

}


void USurvivalWidget::CloseMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("USurvivalWidget::CloseMenu"));
	IsActive = false;
	RemoveFromParent();
	UE_LOG(LogTemp, Warning, TEXT("crash 7"));
	auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	UE_LOG(LogTemp, Warning, TEXT("crash 7"));
	auto Player = (AStoneAgeColonyCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	auto InterfaceManager = Player->InterfaceManager;
	UE_LOG(LogTemp, Warning, TEXT("crash 7"));
	//Player->OpenedMenus.Remove(this);
	UE_LOG(LogTemp, Warning, TEXT("crash 7"));
	int32 Index = InterfaceManager->OpenedMenus.IndexOfByKey(this);
	UE_LOG(LogTemp, Warning, TEXT("crash 7"));
	InterfaceManager->OpenedMenus[Index] = nullptr;
	UE_LOG(LogTemp, Warning, TEXT("crash 7"));
	if (PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("crash 7"));
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
		PlayerController->bEnableClickEvents = false;
		PlayerController->bEnableMouseOverEvents = false;
		UE_LOG(LogTemp, Warning, TEXT("crash 7"));
	}
}