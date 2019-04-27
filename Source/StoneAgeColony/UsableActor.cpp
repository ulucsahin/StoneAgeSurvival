// Fill out your copyright notice in the Description page of Project Settings.

#include "UsableActor.h"

// Sets default values
AUsableActor::AUsableActor(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	MeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	RootComponent = MeshComp;
}

// Called when the game starts or when spawned
void AUsableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUsableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUsableActor::OnBeginFocus(){
	// Used by custom PostProcess to render outlines
	GLog->Log("UsableActor.cpp OnBeginFocus");
	MeshComp->SetRenderCustomDepth(true);
}

void AUsableActor::OnEndFocus() {
	// Used by custom PostProcess to render outlines
	GLog->Log("UsableActor.cpp OnEndFocus");
	MeshComp->SetRenderCustomDepth(false);
}

void AUsableActor::OnUsed(APawn* InstigatorPawn) {
	GLog->Log("im used");
}



