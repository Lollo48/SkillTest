// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetChasingPoint.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTTask_GetChasingPoint : public UBTTaskNode
{
	GENERATED_BODY()
public:
	
	explicit UBTTask_GetChasingPoint(FObjectInitializer const& ObjectInitializer);
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
};
