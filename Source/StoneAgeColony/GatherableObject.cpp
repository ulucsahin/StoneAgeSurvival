// Fill out your copyright notice in the Description page of Project Settings.

#include "GatherableObject.h"
//#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"

AGatherableObject::AGatherableObject(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	//MeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	//Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Environment_Set/Environment/Foliage/Models/Fir_04_Standalone.Fir_04_Standalone"));

	//MeshComp->SetStaticMesh(Mesh);
}

void AGatherableObject::OnUsed(APawn* InstigatorPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("AGatherableObject::OnUsed"));
}