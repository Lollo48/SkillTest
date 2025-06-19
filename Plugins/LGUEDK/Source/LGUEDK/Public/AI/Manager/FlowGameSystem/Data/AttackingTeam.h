// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TeamData.h"
#include "GameFramework/Actor.h"
#include "AttackingTeam.generated.h"

USTRUCT(BlueprintType)
struct LGUEDK_API FAttackingTeam 
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Limit")
	TArray<FTeamData> Teams;

	FAttackingTeam()
	{
		Teams = TArray<FTeamData>();
	}
};
