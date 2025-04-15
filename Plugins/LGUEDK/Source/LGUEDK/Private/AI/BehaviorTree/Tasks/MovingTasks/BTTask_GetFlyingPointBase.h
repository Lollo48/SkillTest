// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetFlyingPointBase.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTTask_GetFlyingPointBase : public UBTTaskNode
{
	GENERATED_BODY()

public:
	
	explicit UBTTask_GetFlyingPointBase(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


protected:
	
	virtual FVector GetTargetLocation(UBehaviorTreeComponent& OwnerComp, APawn* AIPawn,FVector& Direction,float SearchRadius);
	
	virtual bool IsHittingSomething(AActor* ControlledPawn, FVector& StartLocation,FVector& EndLocation);

private:
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector InitialPositionKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector SearchRadiusKey;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetLocationKey;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float SphereRadius = 100.0f;

	bool bCanExplore;

	UPROPERTY(EditAnywhere, Category = "DEBUG")
	bool bDebug = false;
	
};
