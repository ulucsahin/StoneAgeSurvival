// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class STONEAGECOLONY_API GameLoader
{
public:
	GameLoader();
	~GameLoader();

	void LoadGame(APawn* InstigatorPawn);

	template <typename T>
	void SpawnLoadedActors();

	template<typename T> 
	void DestroyActors();

	void UpdateInventoryUI();
};
