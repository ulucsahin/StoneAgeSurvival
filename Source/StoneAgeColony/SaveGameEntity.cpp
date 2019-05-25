// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveGameEntity.h"

USaveGameEntity::USaveGameEntity()
{
	// TODO: Users should be able to save into different slots.
	SaveSlotName = TEXT("TestSaveSlot");
	UserIndex = 0;
}