// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UsableActor.h"
#include "GatherableObject.generated.h"

UCLASS()
class STONEAGECOLONY_API AGatherableObject : public AUsableActor
{
	GENERATED_BODY()
	
public:	

	/* Player is looking at */
	virtual void OnBeginFocus();

	/* Player is no longer looking at */
	virtual void OnEndFocus();

	virtual void OnClicked();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
