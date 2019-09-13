// Fill out your copyright notice in the Description page of Project Settings.

#include "HouseMenu.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"

void UHouseMenu::InitialSetup()
{
	UE_LOG(LogTemp, Warning, TEXT("house menu on da m"));

	FString Desc_ = "Capacity: " + FString::FromInt(5000);
	FText Description = FText::FromString(Desc_);
	RequirementsText->SetText(Description);

}