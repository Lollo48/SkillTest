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

void UEQSUtility::GenerateGridPoints(const FVector& TargetLocation, float InGridRadius, float InStepSize,float ZStep)
{
	EQSWorldSubsystem->GenerateGridPoints(TargetLocation, InGridRadius, InStepSize,ZStep);
}

void UEQSUtility::RegisterEntity(AActor* InEntity)
{
	EQSWorldSubsystem->RegisterEntity(InEntity);
}

void UEQSUtility::UnregisterEntity(AActor* InEntity)
{
	EQSWorldSubsystem ->UnregisterEntity(InEntity);
}

void UEQSUtility::SetRegion(AEQS_Helper* InEQSHelper, const FGameplayTag& InTag)
{
	EQSWorldSubsystem ->SetRegion(InEQSHelper, InTag);
}

TArray<FVector> UEQSUtility::GetPoints(FVector& TargetLocation, float InMinDistance, float InMaxDistance)
{
	return EQSWorldSubsystem ->GetPoints(TargetLocation, InMinDistance, InMaxDistance);
}

FVector UEQSUtility::GetRandomExplorationPoint(const AActor* SelfEntity,const AActor* TargetIgnored, const FVector& InitialSpawnPosition,
	const FVector& CurrentPosition, float SearchRadius, bool bWantExplore)
{
	return EQSWorldSubsystem ->GetRandomExplorationPoint(SelfEntity,TargetIgnored,InitialSpawnPosition, CurrentPosition, SearchRadius, bWantExplore);
}

FVector UEQSUtility::GetBoidExplorationPoint(const AActor* SelfEntity, const FVector& InitialSpawnPosition,
	const FVector& CurrentPosition, float SearchRadius, bool bWantExplore)
{
	return EQSWorldSubsystem ->GetBoidExplorationPoint(SelfEntity, InitialSpawnPosition, CurrentPosition, SearchRadius, bWantExplore);
}

FVector UEQSUtility::GetAttackPoint(const AActor* SelfEntity,const AActor* TargetIgnored,const FVector& TargetLocation, float InMinDistance,
	float InMaxDistance, const float Margin)
{
	return EQSWorldSubsystem ->GetAttackPoint(SelfEntity,TargetIgnored,TargetLocation, InMinDistance, InMaxDistance, Margin);
}

TArray<FVector> UEQSUtility::GetAreaPoints(const FVector& TargetLocation, float InMinDistance, float InMaxDistance,
                                           const FGameplayTag& InTag)
{
	return EQSWorldSubsystem ->GetAreaPoints(TargetLocation, InMinDistance, InMaxDistance, InTag);
}

FVector UEQSUtility::GetRandomExplorationAreaPoint(const AActor* SelfEntity,const AActor* TargetIgnored, const FVector& InitialSpawnPosition,
                                               const FVector& CurrentPosition, float SearchRadius, bool bWantExplore, const FGameplayTag& InTag)
{
	return EQSWorldSubsystem ->GetRandomExplorationAreaPoint(SelfEntity,TargetIgnored,InitialSpawnPosition, CurrentPosition, SearchRadius, bWantExplore, InTag);
}

FVector UEQSUtility::GetBoidExplorationAreaPoint(const AActor* SelfEntity, const FVector& InitialSpawnPosition,
	const FVector& CurrentPosition, float SearchRadius, bool bWantExplore, const FGameplayTag& InTag)
{
	return EQSWorldSubsystem ->GetBoidExplorationAreaPoint(SelfEntity, InitialSpawnPosition, CurrentPosition, SearchRadius, bWantExplore, InTag);
}

FVector UEQSUtility::GetAttackAreaPoint(const AActor* SelfEntity,const AActor* TargetIgnored,const FVector& TargetLocation, float InMinDistance,
	float InMaxDistance, const float Margin, const FGameplayTag& InTag)
{
	return EQSWorldSubsystem ->GetAttackAreaPoint(SelfEntity,TargetIgnored,TargetLocation, InMinDistance, InMaxDistance, Margin, InTag);
}

FVector UEQSUtility::GetChasingAreaPoint(const AActor* SelfEntity,const AActor* TargetIgnored,const FVector& TargetLocation, float InMinDistance, float InMaxDistance)
{
	return EQSWorldSubsystem ->GetChasingAreaPoint(SelfEntity,TargetIgnored,TargetLocation, InMinDistance, InMaxDistance);
}

FVector UEQSUtility::GetChasingPoint(const AActor* SelfEntity,const AActor* TargetIgnored,const FVector& TargetLocation, float InMinDistance, float InMaxDistance)
{
	return EQSWorldSubsystem ->GetChasingPoint(SelfEntity,TargetIgnored,TargetLocation, InMinDistance, InMaxDistance);
}

