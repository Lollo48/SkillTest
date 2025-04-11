// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BTTask_GetEQSBasePoint.h"
#include "BTTask_GetBaseStateEQSPoint.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTTask_GetBaseStateEQSPoint : public UBTTask_GetEQSBasePoint
{
	GENERATED_BODY()

public:
	explicit UBTTask_GetBaseStateEQSPoint(FObjectInitializer const& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FVector PerformTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void InitTask(UBehaviorTreeComponent& OwnerComp) override;

protected:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI", meta=(AllowPrivateAccess = "true"))
	float Margin;
;
};
