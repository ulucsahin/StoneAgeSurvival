// Fill out your copyright notice in the Description page of Project Settings.

#include "UIMainMenu.h"
#include "GameLoadManager.h"

UUIMainMenu::UUIMainMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}


void UUIMainMenu::InitializeVariables()
{
	Loader = NewObject<AGameLoadManager>();
}