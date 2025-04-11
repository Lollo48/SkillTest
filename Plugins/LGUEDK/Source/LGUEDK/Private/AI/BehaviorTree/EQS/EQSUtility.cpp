// Copyright Villains, Inc. All Rights Reserved.


#include "AI/EQS/EQSUtility.h"

UEQS_WorldSubsystem* UEQSUtility::EQSWorldSubsystem = nullptr;

void UEQSUtility::InitWorldSubsystem(UEQS_WorldSubsystem* InEQSWorldSubsystem)
{
	if (!IsValid(InEQSWorldSubsystem))
	{
		UE_LOG(LogTemp, Warning, TEXT("EQSWorldSubsystem is not valid"));
		return;
	}

	EQSWorldSubsystem = InEQSWorldSubsystem;
}

TArray<FVector> UEQSUtility::GenerateGridPoints(FVector& TargetLocation, float InGridRadius, float InStepSize)
{
	return EQSWorldSubsystem->GenerateGridPoints(TargetLocation, InGridRadius, InStepSize);
}

void UEQSUtility::RegisterEntity(AActor* Requester, AActor* Target)
{
	EQSWorldSubsystem->RegisterEntity(Requester, Target);
}

void UEQSUtility::UnregisterEntity(AActor* Requester)
{
	EQSWorldSubsystem->UnregisterEntity(Requester);
}

TArray<FVector> UEQSUtility::TryGetPoint(AActor* Requester, AActor* Target, float InMinDistance, float InMaxDistance)
{
	return EQSWorldSubsystem->TryGetPoint(Requester, Target, InMinDistance, InMaxDistance);
}

void UEQSUtility::RemovePoint(AActor* Target, const FVector& PointToRemove)
{
	EQSWorldSubsystem->RemovePoint(Target, PointToRemove);
}
