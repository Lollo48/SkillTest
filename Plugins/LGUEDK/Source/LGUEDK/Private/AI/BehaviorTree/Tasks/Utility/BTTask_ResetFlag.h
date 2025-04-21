// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ResetFlag.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTTask_ResetFlag : public UBTTaskNode
{
	GENERATED_BODY()

public:
	explicit UBTTask_ResetFlag(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector BoolKey;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	bool bNewValue;
};
