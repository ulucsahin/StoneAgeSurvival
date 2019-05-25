// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class STONEAGECOLONY_API GameSaver
{
public:
	GameSaver();
	~GameSaver();

	void SaveGame(APawn*);

	template <typename T>
	void RegisterActors();
};
