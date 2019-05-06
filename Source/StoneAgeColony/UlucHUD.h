#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
//#include "Runtime/Engine/Classes/Engine/Font.h"
#include "UlucHUD.generated.h"

/**
 * 
 */

USTRUCT()
struct FJoyButtonStruct
{
	GENERATED_USTRUCT_BODY()

	//Vars
	int32 		type;
	FString	toolTip;
	float 		minX;
	float 		maxX;
	float 		minY;
	float 		maxY;

	//~

	//default properties

	FJoyButtonStruct()
	{
		type = -1;
		toolTip = "";
		minX = 0;
		maxX = 0;
		minY = 0;
		maxY = 0;
	}
};


UCLASS()
class STONEAGECOLONY_API AUlucHUD : public AHUD
{
	GENERATED_BODY()

};
