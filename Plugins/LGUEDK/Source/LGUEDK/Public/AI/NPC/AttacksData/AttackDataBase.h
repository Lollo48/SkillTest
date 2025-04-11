// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AttackDataBase.generated.h"

/**
 * 
 */
UCLASS(Abstract,Blueprintable, BlueprintType)
class LGUEDK_API UAttackDataBase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay)
	FGuid AttackDataID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay)
	float AttackDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay)
	float FireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay)
	float ProjectileSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay)
	UParticleSystem* StartAttackParticle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay)
	UParticleSystem* HitAttackParticle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay)
	USoundBase* StartAttackSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay)
	USoundBase* HitAttackSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, AdvancedDisplay)
	UAnimMontage* AttackMontage;

public:
	UAttackDataBase(): AttackDataID(FGuid::NewGuid()),
	                   AttackDamage(0.0f),
	                   FireRate(0.0f),
	                   ProjectileSpeed(0.0f),
	                   StartAttackParticle(nullptr),
	                   HitAttackParticle(nullptr),
	                   StartAttackSound(nullptr),
	                   HitAttackSound(nullptr),
	                   AttackMontage(nullptr)
	{
	}

};
