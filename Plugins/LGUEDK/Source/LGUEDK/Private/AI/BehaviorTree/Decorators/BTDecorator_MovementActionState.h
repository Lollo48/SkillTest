// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Enumerators/ActionState/MovementActionState.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_MovementActionState.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTDecorator_MovementActionState : public UBTDecorator
{
	GENERATED_BODY()

protected:
	explicit UBTDecorator_MovementActionState(FObjectInitializer const& ObjectInitializer);
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	EMovementActionState RequiredState;
};
