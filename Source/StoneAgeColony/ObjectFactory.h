// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "ObjectFactory.generated.h"

class AUsableActor;

USTRUCT(BlueprintType)
struct FObjectCommonPropertiesData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FObjectCommonPropertiesData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ID")
	FString Name_;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	TAssetPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelUp)
	TMap<int32, int32> CraftRequirements;

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

	AUsableActor* CreateObjectBetter(int32 ObjectID);

	FString GetObjectNameFromID(int32 ObjectID);
	
private:
	
	//static AObjectFactory* instance; // Singleton instance
	//TMap<UClass*, UDataTable*> ClassToTable;

	UPROPERTY()
	class UDataTable* CommonPropertiesTable;
	//class UDataTable* EdiblesTable;
	//class UDataTable* EquipmentTable;


	
};
