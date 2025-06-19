// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindRandomAreaLocation.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTTask_FindRandomAreaLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	
	explicit UBTTask_FindRandomAreaLocation(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	
	
	bool bWantExplore;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector SearchRadiusKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector InitialPositionKey;

};
