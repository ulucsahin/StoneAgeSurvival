// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameLoadManager.generated.h"

UCLASS()
class STONEAGECOLONY_API AGameLoadManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameLoadManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void LoadGame(APawn* InstigatorPawn);

	template <typename T>
	void SpawnLoadedActors();

	template<typename T>
	void DestroyActors();

	void RefreshUI(APawn* InstigatorPawn);
};
