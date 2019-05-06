// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivalPlayerController.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

void ASurvivalPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();
	UE_LOG(LogTemp, Warning, TEXT("ASurvivalPlayerController::BeginPlayingState"));

	//UserInterface = CreateWidget<USurvivalWidget>(this, USurvivalWidget::StaticClass());
	//FInputModeGameAndUI Mode;
	//Mode.SetLockMouseToViewport(true);
	//Mode.SetHideCursorDuringCapture(false);
	//SetInputMode(Mode);
	//UserInterface->AddToViewport(9999); // Z-order, this just makes it render on the very top.

	// Add HUD to screen.
	FStringClassReference MyWidgetClassRef(TEXT("/Game/Uluc/HUD/BP_Widget.BP_Widget_C"));
	if (UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>())
	{
		UUserWidget* MyWidget = CreateWidget<UUserWidget>(this, MyWidgetClass);
		// Do stuff with MyWidget
		MyWidget->AddToViewport(9999);
	}


}