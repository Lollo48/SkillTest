// Copyright Villains, Inc. All Rights Reserved.


#include "AI/EQS/EQS_Manager.h"

#include "AI/EQS/EQSUtility.h"


AEQS_Manager::AEQS_Manager()
{

}

void AEQS_Manager::BeginPlay()
{
	Super::BeginPlay();
	if (bWantsGeneratePoints)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this,&AEQS_Manager::GeneratePoints);
	}
}

void AEQS_Manager::GeneratePoints()
{
	FVector TargetLocation = GetActorLocation();
	UEQSUtility::GenerateGridPoints(TargetLocation,GridRadius,StepSize,ZStep);
}

