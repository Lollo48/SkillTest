// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/Enumerators/EnemyState.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_SetState.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTTask_SetState : public UBTTaskNode
{
	GENERATED_BODY()

public:
	
	explicit UBTTask_SetState(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
		
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI", meta=(AllowPrivateAccess = "true"))
	EEnemyState DesiredState;
	
};
