// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralEntityPlacer.h"
#include "GatherableObject.h"
#include "GatherableTree.h"

// Sets default values
AProceduralEntityPlacer::AProceduralEntityPlacer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//// Check this again, this should be unnecessary since we already assign items to spawn in editor.
	//static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/Uluc/ActiveAssets/GatherableObjects/BP_GatherableTree.BP_GatherableTree'"));
	//
	//if (ItemBlueprint.Object) {
	//	MyItemBlueprint = ((UClass*)ItemBlueprint.Object->GeneratedClass);
	//}

}

// Called when the game starts or when spawned
void AProceduralEntityPlacer::BeginPlay()
{
	Super::BeginPlay();
	
	int32 AmountOfItems = ObjectsToSpawn.Num();

	if (AmountOfItems < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("AProceduralEntityPlacer::BeginPlay ObjectToSpawn is null"));
	}
	else
	{
		int32 RandomNumber = FMath::FRandRange(0, AmountOfItems);
		
		UBlueprint* ObjectToSpawn = ObjectsToSpawn[RandomNumber];
		FString ObjectName = "None";
		if (ObjectsToSpawnName.Num() > 0)
		{
			ObjectName = ObjectsToSpawnName[RandomNumber];
		}
		
		auto temp = ObjectToSpawn->GeneratedClass;

		//auto ObjectToSpawn = NewObject<AGatherableObject>();

		
		for (int i = 0; i < HowMany; i++)
		{
			//AGatherableTree* DroppedItem = GetWorld()->SpawnActor<AGatherableTree>(MyItemBlueprint, this->GetActorLocation(), FRotator::ZeroRotator);
			AActor* DroppedItem = GetWorld()->SpawnActor<AActor>(temp, this->GetActorLocation(), FRotator::ZeroRotator);
			if (ObjectName != "None")
			{
				((AUsableActor*)DroppedItem)->SetupType("GatherableTree");
			}

			// Snap spawned object to ground by using line traces.
			AdjustHeight(DroppedItem);

			// Disable tick
			DroppedItem->SetActorTickEnabled(false);
		}

	}

}

// Called every frame
void AProceduralEntityPlacer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AProceduralEntityPlacer::AdjustHeight(AActor* item)
{
	/* This method uses line traces to snap item to ground. */

	// Generate random X and Y for location
	float randX = FMath::RandRange(-AreaRadius, AreaRadius);
	float randY = FMath::RandRange(-AreaRadius, AreaRadius);

	// Z to change
	float TotalZ = 0.f;

	// Get item location
	FVector ActorLocation = item->GetActorLocation();

	// Create Line Trace
	float TraceLength = 1000.f;

	// Set initial start and end positions of line
	FVector Start = FVector(ActorLocation.X + randX, ActorLocation.Y + randY, ActorLocation.Z);
	FVector End = Start;

	FCollisionQueryParams CollisionParams(FName(TEXT("LandscapeTracer")), true, this);
	float DistanceToFloor = 0;
	int iteration = 0;
	FString HittedObjectName = "";
	FHitResult OutHit(ForceInit);

	while (TraceLength > 5.f)
	{
		HittedObjectName = "";

		End.Z -= TraceLength;
		
		bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_WorldStatic, CollisionParams);
		auto a = OutHit.GetActor();

		if (a)
		{
			auto b = a->GetClass();
			if (b)
			{
				HittedObjectName = b->GetName();
			}
		}
		//UE_LOG(LogTemp, Warning, TEXT("AProceduralEntityPlacer::AdjustHeight HITTED OBJECT NAME: %s"), *HittedObjectName);
		// if linetrace hit the landscape
		if (HittedObjectName == "Landscape")
		{

			End.Z += TraceLength;
			TraceLength /= 2;

		}
		// if linetrace did not hit the landscape
		else if(HittedObjectName == "")
		{
			Start = End;
			DistanceToFloor += TraceLength;
		}
		// destroy object if linetrace hit another object, which means there is another object under current object
		else
		{
			item->Destroy();
			break;
		}

		// "end of current loop" operations
		
		iteration++;
		if (iteration > 100)
		{
			item->Destroy();
			break;
		}
	}
	
	// set rotation according to landscape normal, add random yaw
	AdjustToSurfaceNormal(item, &OutHit);

	// place object to calculated position
	item->SetActorLocation(FVector(ActorLocation.X + randX, ActorLocation.Y + randY - 250.f, ActorLocation.Z - DistanceToFloor));
}

void AProceduralEntityPlacer::AdjustToSurfaceNormal(AActor* item, FHitResult* OutHit)
{
	/*
	* This method adjust spawned items' pitch and roll according to normal of surface they are placed on
	* Also adds random yaw value so not all items look same.	
	*/

	auto OriginalRotation = item->GetActorRotation();
	float RandomYaw = FMath::RandRange(0.f, 360.f); // add yaw before calculations
	OriginalRotation.Yaw += RandomYaw;
	FQuat RootQuat = OriginalRotation.Quaternion();
	FVector UpVector = RootQuat.GetUpVector();
	auto Normal = OutHit->ImpactNormal;
	FVector RotationAxis = FVector::CrossProduct(UpVector, Normal);
	RotationAxis.Normalize();
	float DotProduct = FVector::DotProduct(UpVector, Normal);
	float RotationAngle = acosf(DotProduct);
	FQuat Quat = FQuat(RotationAxis, RotationAngle);
	FQuat NewQuat = Quat * RootQuat;

	auto NewRotation = NewQuat.Rotator();
	NewRotation.Pitch *= 0.2f;
	NewRotation.Roll *= 0.2f;
	item->SetActorRotation(NewRotation);
}