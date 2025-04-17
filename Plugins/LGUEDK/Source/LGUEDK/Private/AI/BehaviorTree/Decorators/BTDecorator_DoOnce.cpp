// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_DoOnce.h"

UBTDecorator_DoOnce::UBTDecorator_DoOnce(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Do Once";
}

bool UBTDecorator_DoOnce::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (!bIsAlreadyDone)
	{
		bIsAlreadyDone = true;
		return true;
	}

	return false;
}

