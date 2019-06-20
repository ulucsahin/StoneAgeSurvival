// Fill out your copyright notice in the Description page of Project Settings.

#include "GatherableObject.h"

// Called when the game starts or when spawned
void AGatherableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGatherableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGatherableObject::OnBeginFocus()
{

}

void AGatherableObject::OnEndFocus()
{

}

void AGatherableObject::OnClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("AGatherableObject::OnClicked"));
}


