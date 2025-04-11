// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetBool.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTTask_SetBool : public UBTTaskNode
{
	GENERATED_BODY()


public:
	explicit UBTTask_SetBool(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector BoolKey;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	bool bNewValue;
	
};
