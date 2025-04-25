// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/Enumerators/ComboAttack/ComboAttackState.h"
#include "Engine/DataAsset.h"
#include "AttackDataAsset.generated.h"

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class LGUEDK_API UAttackDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EComboAttackState AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGuid AttackDataID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Priority;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanPerformAttack;
	
	UAttackDataAsset(): AttackType(),
			           AttackDataID(FGuid::NewGuid()),
	                   Damage(0.0f),
	                   Cooldown(0.0f),
	                   MinRange(0.0f),
	                   MaxRange(0.0f),
	                   Priority(0.0f),
	                   AttackMontage(nullptr),
						bCanPerformAttack(true)
	{
	}
	
};
