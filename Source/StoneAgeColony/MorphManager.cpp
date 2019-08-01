// Fill out your copyright notice in the Description page of Project Settings.

#include "MorphManager.h"

// Sets default values for this component's properties
UMorphManager::UMorphManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UMorphManager::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UMorphManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
void UMorphManager::SetupManager(ACharacter* Owner)
{
	/* Initial setup when creating new character */
	
	this->Owner = Owner;
	if (Owner)
	{
		USkeletalMeshComponent* Mesh = Owner->GetMesh();
		if (Mesh)
		{
			//TMap<FName, float> tMap = animInstance->MorphTargetCurves;
			Mesh->SetMorphTarget("NoseHeight", 1.f);
			Mesh->SetMorphTarget("MouthHeight", -1.f);
			
			float RandomValue = FMath::FRandRange(-1.f, 1.f);

			USkeletalMesh* SkelMesh = Mesh->SkeletalMesh;
			
			for (auto x : SkelMesh->MorphTargetIndexMap)
			{
				auto asd = x.Key.ToString();
				
				float RandomValue = FMath::FRandRange(-1.f, 1.f);
				Mesh->SetMorphTarget(*asd, RandomValue);

				UE_LOG(LogTemp, Warning, TEXT("morph name: %s"), *asd);
				UE_LOG(LogTemp, Warning, TEXT("morph value: %f"), RandomValue);
			}
			
			//for (auto x : Mesh->GetMorphTargetCurves())
			//{
			//	auto asd = x.Key.ToString();
			//	UE_LOG(LogTemp, Warning, TEXT("morph name: %s"), *asd);
			//}

		}

	}

}

