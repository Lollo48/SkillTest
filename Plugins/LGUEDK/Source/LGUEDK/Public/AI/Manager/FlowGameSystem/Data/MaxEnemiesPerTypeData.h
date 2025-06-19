// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/Enumerators/EEnemyType.h"
#include "GameFramework/Actor.h"
#include "MaxEnemiesPerTypeData.generated.h"

USTRUCT(BlueprintType)
struct LGUEDK_API FMaxEnemiesPerTypeData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EEnemyType> EnemyType;

	/**
	 * Maximum number of attackers of this type allowed at the same time.
	 */
	UPROPERTY(EditAnywhere)
	int32 MaxAttackersSimultaneous;

	FMaxEnemiesPerTypeData(): EnemyType()
	{
		MaxAttackersSimultaneous = 0;
	}
};
