// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveFlyingEnemy.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTTask_MoveFlyingEnemy : public UBTTaskNode
{
	GENERATED_BODY()

protected:

	explicit UBTTask_MoveFlyingEnemy(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	float CharacterSpeed = 0.0f;
	
private:

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetLocationKey;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float AcceptanceRadius = 5.0f;  
	
	FVector TargetLocation;
};
