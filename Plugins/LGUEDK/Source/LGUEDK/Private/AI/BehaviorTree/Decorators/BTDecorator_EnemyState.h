// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/Enumerators/EnemyState.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_EnemyState.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTDecorator_EnemyState : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
protected:
	explicit UBTDecorator_EnemyState(FObjectInitializer const& ObjectInitializer);
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	EEnemyState RequiredState;
};
