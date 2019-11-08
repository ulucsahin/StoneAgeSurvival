// Fill out your copyright notice in the Description page of Project Settings.

#include "SpecifyCraftListItem.h"
#include "Components/Slider.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"

void USpecifyCraftListItem::InitialSetup()
{
	
	ItemNameBlock->SetText(FText::FromString("asdasdas"));
	//Slider->OnValueChanged.AddDynamic(this, &USpecifyCraftListItem::OnSliderValueChanged);
	//Slider->OnValueChanged.
	//auto x = FName("asd");
	//Slider->OnValueChanged.AddDynamic(this, &USpecifyCraftListItem::OnSliderValueChanged, x);
}

// Called from blueprint
void USpecifyCraftListItem::OnSliderValueChanged()
{
	UE_LOG(LogTemp, Warning, TEXT("USpecifyCraftListItem::OnSliderValueChanged"));
}