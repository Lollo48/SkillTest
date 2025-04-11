// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WaveDataAsset.h"
#include "GameFramework/Actor.h"
#include "WaveQuantity.generated.h"

USTRUCT(BlueprintType)
struct LGUEDK_API FWaveQuantity 
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waves")
	TArray<UWaveDataAsset*> Waves;

	FWaveQuantity()
	{
		Waves = TArray<UWaveDataAsset*>();
	} 
};
