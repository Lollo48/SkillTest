// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_IsWithInRange.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTDecorator_IsWithInRange : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
protected:
	
	explicit UBTDecorator_IsWithInRange(FObjectInitializer const& ObjectInitializer);
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector DistanceFromPlayerKey;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector MaxAttackRadiusKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector MinAttackRadiusKey;
};
