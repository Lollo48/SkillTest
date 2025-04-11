// Copyright Villains, Inc. All Rights Reserved.


#include "BTDecorator_CheckBool.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CheckBool::UBTDecorator_CheckBool(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Check Bool";
}

bool UBTDecorator_CheckBool::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{

	return OwnerComp.GetBlackboardComponent()->GetValueAsBool(BoolKey.SelectedKeyName);
	
}
