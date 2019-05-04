// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"

/**
 * 
 */
class STONEAGECOLONY_API GameLoader
{
public:
	GameLoader();
	~GameLoader();

	void LoadGame();
	void SpawnCharacters();

	template<typename T> 
	void DestroyActors();
};
