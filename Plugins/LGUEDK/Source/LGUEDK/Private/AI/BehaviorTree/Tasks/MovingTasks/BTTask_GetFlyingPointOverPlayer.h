// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BTTask_GetFlyingPointBase.h"
#include "BTTask_GetFlyingPointOverPlayer.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTTask_GetFlyingPointOverPlayer : public UBTTask_GetFlyingPointBase
{
	GENERATED_BODY()

protected:
	
	explicit UBTTask_GetFlyingPointOverPlayer(FObjectInitializer const& ObjectInitializer);
	virtual FVector GetTargetLocation(UBehaviorTreeComponent& OwnerComp, APawn* AIPawn,FVector& Direction,float SearchRadius) override;

private:
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector AttackTargetKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector MaxDistanceKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector MinDistanceKey;
	
	UPROPERTY(EditAnywhere, Category = "Parameters")
	float MaxHeight;

};
