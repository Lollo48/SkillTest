// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TriggerBase/BaseTrigger.h"
#include "BTTask_GetBaseTrigger.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTTask_GetBaseTrigger : public UBTTaskNode
{
	GENERATED_BODY()

public:
	explicit UBTTask_GetBaseTrigger(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FGameplayTag BaseTriggerTag;

private:
	ABaseTrigger* GetAvaiableTrigger();
	
};
