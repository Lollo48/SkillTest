// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_GetFlyingPointOverPlayer.h"

#include "BehaviorTree/BlackboardComponent.h"


UBTTask_GetFlyingPointOverPlayer::UBTTask_GetFlyingPointOverPlayer(FObjectInitializer const& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Get Flying Point Over Player";
}

FVector UBTTask_GetFlyingPointOverPlayer::TryFindFlyingTargetLocation(UBehaviorTreeComponent& OwnerComp, APawn* AIPawn,FVector& Direction,float SearchRadius)
{
	if (!AIPawn) { return FVector::ZeroVector; }

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) { return FVector::ZeroVector; }

	AActor* Player = Cast<AActor>(Blackboard->GetValueAsObject(AttackTargetKey.SelectedKeyName));
	if (!Player) { return FVector::ZeroVector; }

	FVector PlayerLocation = Player->GetActorLocation();

	float MaxDistance = Blackboard->GetValueAsFloat(MaxDistanceKey.SelectedKeyName);
	float MinDistance = Blackboard->GetValueAsFloat(MinDistanceKey.SelectedKeyName);

	if (MinDistance > MaxDistance) 
	{
		MinDistance = MaxDistance * 0.5f; 
	}
	
	FVector TargetLocation;

	float Angle = FMath::RandRange(0.0f, 360.0f);
	float Distance = FMath::RandRange(MinDistance, MaxDistance); 

	FVector PossibleDirection = FVector(FMath::Cos(FMath::DegreesToRadians(Angle)), FMath::Sin(FMath::DegreesToRadians(Angle)), 0);

	TargetLocation = PlayerLocation + PossibleDirection * Distance;

	float MinHeight = PlayerLocation.Z + 150.0f;
	MaxHeight = MinHeight + MaxDistance;
	TargetLocation.Z = FMath::RandRange(MinHeight, MaxHeight);
	
	return TargetLocation;
}
