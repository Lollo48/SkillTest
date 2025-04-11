// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnemySpawnInfo.h"
#include "Engine/DataAsset.h"
#include "WaveDataAsset.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class LGUEDK_API UWaveDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<FEnemySpawnInfo> EnemiesToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	float TimeBeforeNextWave = 5.0f; 
};
