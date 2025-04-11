// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NPCBaseEnemyDataAsset.h"
#include "AI/Enumerators/EEnemyType.h"
#include "Engine/DataAsset.h"
#include "NPCBaseStateEnemyDataAsset.generated.h"


UCLASS()
class LGUEDK_API UNPCBaseStateEnemyDataAsset : public UNPCBaseEnemyDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<EEnemyType> MyEnemyType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanStatePassive;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanStatePatrolling;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanJump;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanStateInvestigating;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanStateChasing;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanStatePending;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanStateAttacking;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanStateDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Patrolling",meta = ( EditCondition = "bCanStatePatrolling",EditConditionHides))
	float SearchRadius = 200;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Patrolling", meta = (ClampMin = "0", ClampMax = "10", UIMin = "0", UIMax = "10", EditCondition = "bCanStatePatrolling",EditConditionHides))
	float TimeBeforeNextStep = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Patrolling", meta = ( EditCondition = "bCanStatePatrolling",EditConditionHides))
	bool bWantExplore;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Investigating", meta = (ClampMin = "0", ClampMax = "1000", UIMin = "0", UIMax = "1000", EditCondition = "bCanStateInvestigating",EditConditionHides))
	float MinInvestigatingRadius = 200;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Investigating", meta = (ClampMin = "0", ClampMax = "2000", UIMin = "0", UIMax = "2000", EditCondition = "bCanStateInvestigating",EditConditionHides))
	float MaxInvestigatingRadius = 600;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Investigating", meta = (ClampMin = "0", ClampMax = "10", UIMin = "0", UIMax = "10", EditCondition = "bCanStateInvestigating",EditConditionHides))
	float TimeBeforeInvestigating = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Investigating", meta = (ClampMin = "0", ClampMax = "10", UIMin = "0", UIMax = "10", EditCondition = "bCanStateInvestigating",EditConditionHides))
	float RandomInvestigatingTimeDeviation = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Jumping",meta =( EditCondition = "bCanJump",EditConditionHides))
	float MaxJumpingHeight = 500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Jumping",meta =( EditCondition = "bCanJump",EditConditionHides))
	float MinJumpingDistance = 500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Attack Ranges",meta =( EditCondition = "bCanStateAttacking",EditConditionHides))
	float MinAttackRadius = 500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Attack Ranges",meta =( EditCondition = "bCanStateAttacking",EditConditionHides))
	float MaxAttackRadius = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Pending",meta =( EditCondition = "bCanStatePending",EditConditionHides))
	float MinPendingRadius = 1000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Pending",meta =( EditCondition = "bCanStatePending",EditConditionHides))
	float MaxPendingRadius = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|FeedBack")
	UParticleSystem* SpawnEntityEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|FeedBack")
	UParticleSystem* DeathEntityEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|FeedBack")
	USoundBase* SpawnEntitySound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|FeedBack")
	USoundBase* DeathEntitySound;
};
