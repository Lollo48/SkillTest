// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_DoOnce.generated.h"


/**
 * 
 */
UCLASS()
class LGUEDK_API UBTDecorator_DoOnce : public UBTDecorator
{
	GENERATED_BODY()

protected:
	
	explicit UBTDecorator_DoOnce(FObjectInitializer const& ObjectInitializer);
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:

	UPROPERTY(EditAnywhere, Category = "AI|Blackboard")
	FBlackboardKeySelector ResetFlagKey;
	
	UPROPERTY()
	mutable bool bIsAlreadyDone = false;
	
};
