// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetEQSBasePoint.generated.h"


UCLASS()
class LGUEDK_API UBTTask_GetEQSBasePoint : public UBTTaskNode
{
	GENERATED_BODY()

public:
	
	explicit UBTTask_GetEQSBasePoint(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector AttackTargetKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector ControlledPawnKey;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector MinDistanceKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector MaxDistanceKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetLocationKey;
	
protected:
	
	virtual FVector PerformTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	
	virtual void InitTask(UBehaviorTreeComponent& OwnerComp);
	
	bool IsHittingSomething(const FVector& Start, const FVector& End) const;

	bool IsPointFree(const FVector& Point) const;
	
	UPROPERTY()
	AActor* AttackTarget;
	UPROPERTY()
	ANPCBaseStateEnemy* ControlledPawn;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI", meta=(AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionChannel> ObjectTypeChannelPointFree;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI", meta=(AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionChannel> CollisionChannelIsHittingSomething;
	
	UPROPERTY()
	float MinDistance;
	
	UPROPERTY()
	float MaxDistance;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI", meta=(AllowPrivateAccess = "true"))
	float DistanceBetweenEnemyRadius;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI", meta=(AllowPrivateAccess = "true"))
	bool bShowDebug;

};

