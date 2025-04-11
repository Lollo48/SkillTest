// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnemyQuantityInfo.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnInfo.generated.h"

USTRUCT(BlueprintType)
struct LGUEDK_API FEnemySpawnInfo 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TArray<FEnemyQuantityInfo> ListEnemyToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	int32 SpawnPointIndex;
	
	FEnemySpawnInfo(): SpawnPointIndex(0)
	{
		ListEnemyToSpawn = TArray<FEnemyQuantityInfo>();
	}
};

