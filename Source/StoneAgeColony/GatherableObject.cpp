// Fill out your copyright notice in the Description page of Project Settings.

#include "GatherableObject.h"

// Sets default values
AGatherableObject::AGatherableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

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


