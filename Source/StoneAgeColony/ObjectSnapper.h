// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "UsableActor.h"
#include "ObjectSnapper.generated.h"

class AUsableActor;

UCLASS()
class STONEAGECOLONY_API AObjectSnapper : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectSnapper();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float CalculateHeight(AUsableActor*);
	void SnapToGround(AUsableActor*, UWorld*, FVector, float Threshold);
	void SnapReverse(AUsableActor*, UWorld*, FVector, float Threshold);

};
