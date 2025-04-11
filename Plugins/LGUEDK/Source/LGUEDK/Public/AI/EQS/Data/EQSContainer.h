// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EQSContainer.generated.h"

USTRUCT(BlueprintType)
struct LGUEDK_API FEQSContainer 
{
	GENERATED_BODY()
	
	TArray<FVector> Points;

	FEQSContainer(): Points(TArray<FVector>()){}
};
