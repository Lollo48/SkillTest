// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/Enumerators/ActionState/FlyingMode.h"
#include "AI/Enumerators/Directions/DirectionPreference.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindFlyingTargetLocation.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UBTTask_FindFlyingTargetLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	
	explicit UBTTask_FindFlyingTargetLocation(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	
	virtual FVector TryFindFlyingTargetLocation(UBehaviorTreeComponent& OwnerComp, APawn* AIPawn,FVector& Direction,float SearchRadius);
	virtual bool IsHittingSomething(FVector& StartLocation,FVector& EndLocation);

private:

	UPROPERTY()
	APawn* ControlledPawn;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard",meta = (EditCondition = "bWantExplore"))
	FBlackboardKeySelector InitialPositionKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard",meta = (EditCondition = "EFlyingMode == EFlyingMode::Random"))
	FBlackboardKeySelector SearchRadiusKey;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetLocationKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard",meta = (EditCondition = "EFlyingMode == EFlyingMode::AroundActor"))
	FBlackboardKeySelector AroundActorKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard",meta = (EditCondition = "EFlyingMode == EFlyingMode::AroundActor"))
	FBlackboardKeySelector MaxDistanceFromActorKey;

	UPROPERTY(EditAnywhere, Category = "Blackboard",meta = (EditCondition = "EFlyingMode == EFlyingMode::AroundActor"))
	FBlackboardKeySelector MinDistanceFromActorKey;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Flying|Settings", meta=(AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionChannel> CollisionChannelIsHittingSomething;

	UPROPERTY(EditAnywhere, Category="Flying|General")
	EFlyingMode FlyingMode = EFlyingMode::Random;

	UPROPERTY(EditAnywhere, Category = "Flying|General")
	float MinAltitudeFromGround = 200.f;

	UPROPERTY(EditAnywhere, Category = "Flying|General")
	float MaxAltitudeFromGround = 800.f;

	UPROPERTY(EditAnywhere, Category = "Flying|General",meta = (EditCondition = "EFlyingMode == EFlyingMode::Circular"))
	float FlyingCircleRadius = 300.f; 

	UPROPERTY(EditAnywhere, Category = "Flying|General",meta = (EditCondition = "EFlyingMode == EFlyingMode::Circular"))
	float CircleAngleStep = 30.f; 
	
	UPROPERTY(EditAnywhere, Category = "Flying|General")
	float MinYaw = -150.0f;

	UPROPERTY(EditAnywhere, Category = "Flying|General")
	float MaxYaw = 150.0f;

	UPROPERTY(EditAnywhere, Category = "Flying|General")
	float MinPitch = -100.0f;

	UPROPERTY(EditAnywhere, Category = "Flying|General")
	float MaxPitch = 100.0f;
	
	UPROPERTY(EditAnywhere, Category = "Flying|Settings")
	float SphereRadius = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Flying|General")
	EDirectionPreference DirectionPreference = EDirectionPreference::Any;

	UPROPERTY(EditAnywhere, Category = "Flying|General")
	float VerticalBias = 0.0f; // -1 = in basso, 1 = in alto

	UPROPERTY(EditAnywhere, Category = "Flying|General",meta = (EditCondition = "EFlyingMode == EFlyingMode::random"))
	float MinRotationYaw = -150.f;
	UPROPERTY(EditAnywhere, Category = "Flying|General",meta = (EditCondition = "EFlyingMode == EFlyingMode::random"))
	float MaxRotationYaw = 150.f;
	UPROPERTY(EditAnywhere, Category = "Flying|General",meta = (EditCondition = "EFlyingMode == EFlyingMode::random"))
	float MinRotationPitch = -100.f;
	UPROPERTY(EditAnywhere, Category = "Flying|General",meta = (EditCondition = "EFlyingMode == EFlyingMode::random"))
	float MaxRotationPitch = 100.f;
	

	bool bCanExplore;

	FVector CurrentHitLocation;

	UPROPERTY(EditAnywhere, Category = "Flying|Settings")
	bool bDebug = false;

	UPROPERTY()
	float CurrentAngle = 0.f;

	UPROPERTY()
	int Iteration = 0;

	UFUNCTION()
	FVector GetRandomDirectionFromPreference() const;

	UFUNCTION()
	float GetAltitudeAboveGround(const FVector& Location);
	
	UFUNCTION()
	FVector GetRandomFlyingLocation(const FVector& StartLocation, const FVector& InitialPosition, const FVector& Direction, float SearchRadius);

	UFUNCTION()
	FVector GetCircularFlyingLocation(const FVector& CenterRadius);

	UFUNCTION()
	FVector GetAroundActorLocation(UBlackboardComponent* Blackboard, const FVector& StartLocation);

	UFUNCTION()
	FVector GetToPoint(const FVector& StartLocation, const FVector& EndLocation);
	
};



