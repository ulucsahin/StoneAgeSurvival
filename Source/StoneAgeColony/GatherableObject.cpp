// Fill out your copyright notice in the Description page of Project Settings.

#include "GatherableObject.h"

AGatherableObject::AGatherableObject(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void AGatherableObject::OnUsed(APawn* InstigatorPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("AGatherableObject::OnUsed"));
}