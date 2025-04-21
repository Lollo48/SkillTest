// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Enumerators/ActionState/FlyingActionState.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_FlyingActionState.generated.h"

/**
 * 
 */
UCLASS()
class SKILLTESTPROJECT_API UBTDecorator_FlyingActionState : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTDecorator_FlyingActionState(FObjectInitializer const& ObjectInitializer);
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	
private:
		
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI", meta=(AllowPrivateAccess = "true"))
	EFlyingActionState RequiredState;
};
