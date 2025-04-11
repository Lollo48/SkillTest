// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AI/NPC/NPCBase/NPCBase.h"
#include "EnemyQuantityInfo.generated.h"

USTRUCT(BlueprintType)
struct LGUEDK_API FEnemyQuantityInfo 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	TArray<TSubclassOf<ANPCBase>> EnemyClasses;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
	int32 Quantity;

	FEnemyQuantityInfo(): Quantity(0)
	{
		EnemyClasses = TArray<TSubclassOf<ANPCBase>>();
	}
};
