// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	
private:

	UPROPERTY(EditAnywhere, Category = "AI|Blackboard")
	FBlackboardKeySelector TargetLocationKey;

	UPROPERTY(EditAnywhere, Category = "AI|Movement")
	float IncrementMovementSpeed = 4000.0f;

	UPROPERTY(EditAnywhere, Category = "AI|Movement")
	float BrakingDecelerationFlying = 2000.f;

	UPROPERTY(EditAnywhere, Category = "AI|Movement")
	bool bUseControllerDesiredRotation = false;

	UPROPERTY(EditAnywhere, Category = "AI|Movement")
	bool bOrientRotationToMovement = false;

	UPROPERTY(EditAnywhere, Category = "AI|Movement")
	float AcceptanceRadius = 5.0f;

	UPROPERTY(EditAnywhere, Category = "AI|Settings")
	bool bDebug;
	
	FVector TargetLocation;

	UPROPERTY()
	APawn* Entity;
	
};
