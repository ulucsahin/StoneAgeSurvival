// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalWidget.h"
#include "Runtime/UMG/Public/Components/Button.h"

void USurvivalWidget::NativeConstruct()
{
	Super::NativeConstruct();
}


void USurvivalWidget::CloseMenu()
{
	IsActive = false;
	RemoveFromParent();
}