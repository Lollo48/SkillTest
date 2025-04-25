// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PerformComboAttack.generated.h"

/**
 * 
 */
UCLASS()
class SKILLTESTPROJECT_API UBTTask_PerformComboAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	
	explicit UBTTask_PerformComboAttack(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector ControlledPawn;
};
