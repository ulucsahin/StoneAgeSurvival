// Fill out your copyright notice in the Description page of Project Settings.

/*
This object is responsible from saving the game.
Game will be saved when player uses bed.
*/

#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "ObjectBed.generated.h"

/**
 * 
 */
UCLASS()
class STONEAGECOLONY_API AObjectBed : public AUsableActor
{
	GENERATED_BODY()
public:
	AObjectBed(const FObjectInitializer& ObjectInitializer);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void OnUsed(APawn* InstigatorPawn) override;
};
