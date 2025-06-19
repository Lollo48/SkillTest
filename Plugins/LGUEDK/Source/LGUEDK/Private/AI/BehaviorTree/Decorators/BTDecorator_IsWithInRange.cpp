// Copyright Villains, Inc. All Rights Reserved.


#include "BTDecorator_IsWithInRange.h"

#include <string>
#include "BehaviorTree/BlackboardComponent.h"


UBTDecorator_IsWithInRange::UBTDecorator_IsWithInRange(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Is With In Range";

	bAllowAbortNone = true;
	bAllowAbortLowerPri = true;
	bAllowAbortChildNodes = true;
}

bool UBTDecorator_IsWithInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp)
	{
		return false;
	}
	
	const float MinRange = BlackboardComp->GetValueAsFloat(MinAttackRadiusKey.SelectedKeyName);
	float DistanceFromPlayer = BlackboardComp->GetValueAsFloat(DistanceFromPlayerKey.SelectedKeyName);
	
	if (DistanceFromPlayer < MinRange)
	{
		return true; 
	}
	
	return false;
}
