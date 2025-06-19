// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MaxEnemiesPerTypeData.h"
#include "GameplayTagContainer.h"
#include "AI/Enumerators/EEnemyType.h"
#include "GameFramework/Actor.h"
#include "TeamData.generated.h"

USTRUCT(BlueprintType)
struct LGUEDK_API FTeamData 
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Limit")
	FGameplayTag TeamID;

	/**
	 * list of Enemies Per Type 
	 */
	UPROPERTY(EditAnywhere, Category = "Limit")
	TArray<FMaxEnemiesPerTypeData> MaxEnemiesPerType;

	FTeamData()
	{
		TeamID = FGameplayTag();
		MaxEnemiesPerType = TArray<FMaxEnemiesPerTypeData>();
	}
};
