// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"

// Objects that will be spawned by ObjectFactory
#include "Equipment.h"
#include "GatherableTree.h"

#include "ObjectFactory.generated.h"


USTRUCT(BlueprintType)
struct FObjectNameData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FObjectNameData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ID")
	FString Name;

};


UCLASS()
class STONEAGECOLONY_API AObjectFactory : public AActor
{
	GENERATED_BODY()
	
public:	
	AObjectFactory();
	//static AObjectFactory* GetInstance(); // Singleton instance

	template <typename T>
	T* CreateObject(int32 ObjectID);

	
private:
	
	//static AObjectFactory* instance; // Singleton instance
	//TMap<UClass*, UDataTable*> ClassToTable;

	class UDataTable* IDtoNameTable;
	//class UDataTable* EquipmentTable;


	
};
