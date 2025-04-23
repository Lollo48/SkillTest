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

	/**Put Max Distance From Target Location to 0 to use the DATA asset variable.*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI", meta=(AllowPrivateAccess = "true"))
	float MaxDistanceFromTargetLocation = 0.f;

	/**Put Min Distance From Target Location to 0 to use the DATA asset variable.*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI", meta=(AllowPrivateAccess = "true"))
	float MinDistanceFromTargetLocation = 0.f;
	
};
