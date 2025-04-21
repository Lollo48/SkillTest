// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Enumerators/ActionState/MovementActionState.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetMovementActionState.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTTask_SetMovementActionState : public UBTTaskNode
{
	GENERATED_BODY()

public:
	explicit UBTTask_SetMovementActionState(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI", meta=(AllowPrivateAccess = "true"))
	EMovementActionState MovementActionState;
};
