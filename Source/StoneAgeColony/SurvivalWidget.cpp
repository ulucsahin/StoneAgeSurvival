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
	IsActive = false;
	RemoveFromParent();
	auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	auto Player = (AStoneAgeColonyCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	auto InterfaceManager = Player->InterfaceManager;

	//Player->OpenedMenus.Remove(this);
	int32 Index = InterfaceManager->OpenedMenus.IndexOfByKey(this);
	InterfaceManager->OpenedMenus[Index] = nullptr;
	InterfaceManager->SetInputModeAuto();
}