// Fill out your copyright notice in the Description page of Project Settings.

#include "UsableActor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "StoneAgeColonyCharacter.h"
#include "Communicator.h"

// Sets default values
AUsableActor::AUsableActor(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	MeshComp = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	RootComponent = MeshComp;

	// Set InventoryTexture
	//static ConstructorHelpers::FObjectFinder<UTexture2D> InventoryTexObj(TEXT("Texture2D'/Game/Uluc/HUD/Inventory/lol.lol'"));
	//InventoryTexture = InventoryTexObj.Object;
}

void AUsableActor::PreInitializeComponents() {
	Super::PreInitializeComponents();
	//UE_LOG(LogTemp, Warning, TEXT("AUsableActor::PreInitializeComponents"));
}

// Called when the game starts or when spawned
void AUsableActor::BeginPlay()
{
	Super::BeginPlay();
	//Communicator::GetInstance().UsableItemIDMap.Add(AUsableActor::StaticClass()->GetDefaultObject<AUsableActor>()->ID, AUsableActor::StaticClass());
	UE_LOG(LogTemp, Warning, TEXT("AUsableActor::BeginPlay"));
}

// Called every frame
void AUsableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUsableActor::OnBeginFocus(){
	// Used by custom PostProcess to render outlines
	MeshComp->SetRenderCustomDepth(true);
}

void AUsableActor::OnEndFocus() {
	// Used by custom PostProcess to render outlines
	MeshComp->SetRenderCustomDepth(false);
}


void AUsableActor::OnUsed(APawn* InstigatorPawn) {
	GLog->Log("im used");
	//AUsableActor* temp = NewObject<AUsableActor>();
	((AStoneAgeColonyCharacter*)InstigatorPawn)->AddToInventory(this->ID);
	((AStoneAgeColonyCharacter*)InstigatorPawn)->PrintInventory();
}

void AUsableActor::PrintName() {
	UE_LOG(LogTemp, Warning, TEXT("IM USABLE ACTOR XD"));
}

