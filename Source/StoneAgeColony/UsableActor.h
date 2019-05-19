// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UsableActor.generated.h"

UCLASS()
class STONEAGECOLONY_API AUsableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	//AUsableActor();
	AUsableActor(const FObjectInitializer& ObjectInitializer);
	
	const static int ID = 0;
	UTexture2D* InventoryTexture;

	virtual void OnUsed(APawn* InstigatorPawn);

	/* Player is looking at */
	virtual void OnBeginFocus();

	/* Player is no longer looking at */
	virtual void OnEndFocus();
	
	virtual void PrintName();
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* MeshComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PreInitializeComponents() override;

};
