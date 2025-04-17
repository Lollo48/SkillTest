// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_FindFlyingTargetLocation.h"

#include "AI/NPC/NPCBase/NPCBaseController.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utility/LGDebug.h"

UBTTask_FindFlyingTargetLocation::UBTTask_FindFlyingTargetLocation(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Get Flying Point";
}

EBTNodeResult::Type UBTTask_FindFlyingTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANPCBaseController* const Controller = Cast<ANPCBaseController>(OwnerComp.GetAIOwner());
	if (!Controller)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	APawn* const ControlledPawn = Controller->GetPawn();
	if (!ControlledPawn)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}
	
	float SearchRadius = Blackboard->GetValueAsFloat(SearchRadiusKey.SelectedKeyName);
	bool bFound = false;
	FVector TargetLocation = FVector::ZeroVector;
	FVector ActorPosition = ControlledPawn->GetActorLocation();
	
	for (int i = 0; i < 10; ++i)
	{
		FVector Direction = GetRandomDirectionFromPreference(ControlledPawn);
		TargetLocation = TryFindFlyingTargetLocation(OwnerComp, ControlledPawn, Direction, SearchRadius);
		
		if (bDebug)
		{
			DrawDebugLine(GetWorld(), ActorPosition, TargetLocation, FColor::Red, false, 1.5f, 0, 1.f);
			DrawDebugSphere(GetWorld(), TargetLocation, 400.f, 12, FColor::Yellow, false, 1.5f);
		}
		
		bool bHit = IsHittingSomething(ControlledPawn, ActorPosition, TargetLocation);
		
		if (!bHit)
		{
			bFound = true;
			break;
		}
		if (FlyingMode == EFlyingMode::Circular)
		{
			LGDebug::Log("FlyingMode: Circular e sto hittando ",true);
			const int MaxAdjustTries = 10;
			const float AngleAdjustStep = 40.f; 
			const float AdjustDistance = 150.f;

			for (int j = 1; j <= MaxAdjustTries; ++j)
			{
				float AdjustedAngle = (j % 2 == 0 ? 1 : -1) * AngleAdjustStep * j; 
				float AngleRad = FMath::DegreesToRadians(AdjustedAngle);

				FVector Offset;
				Offset.X = FMath::Cos(AngleRad) * AdjustDistance;
				Offset.Y = FMath::Sin(AngleRad) * AdjustDistance;

				FVector AdjustedTarget = CurrentHitLocation + Offset;
				AdjustedTarget.Z = CurrentHitLocation.Z; 

				if (!IsHittingSomething(ControlledPawn, CurrentHitLocation, AdjustedTarget))
				{
					if (bDebug)
					{
						DrawDebugSphere(GetWorld(), AdjustedTarget, 40.f, 16, FColor::Yellow, false, 2.5f);
						DrawDebugLine(GetWorld(), CurrentHitLocation, AdjustedTarget, FColor::Yellow, false, 2.5f, 0, 2.0f);
					}

					TargetLocation = AdjustedTarget;
					bFound = true;

					// NON aggiorno CurrentAngle! Lo lascio per il prossimo step
					break;
				}
			}
			break;
		}
	}
	
	if (!bFound)
	{
		TargetLocation = ActorPosition;
	}
	
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(TargetLocationKey.SelectedKeyName, TargetLocation);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;	
	
}

FVector UBTTask_FindFlyingTargetLocation::TryFindFlyingTargetLocation(UBehaviorTreeComponent& OwnerComp, APawn* AIPawn,FVector& Direction,float SearchRadius)
{
	if (!AIPawn) return FVector::ZeroVector;

	ANPCBaseStateEnemy* BaseStateEnemy = Cast<ANPCBaseStateEnemy>(AIPawn);
	if (!BaseStateEnemy) return FVector::ZeroVector;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return FVector::ZeroVector;

	FVector InitialPosition = Blackboard->GetValueAsVector(InitialPositionKey.SelectedKeyName);
	bCanExplore = BaseStateEnemy->GetDataAsset()->bWantExplore;

	FVector StartLocation = AIPawn->GetActorLocation();
	FVector TargetLocation = FVector::ZeroVector;

	switch (FlyingMode)
	{
		case EFlyingMode::Random:
			TargetLocation = GetRandomFlyingLocation(StartLocation, InitialPosition, Direction, SearchRadius);
		break;
		case EFlyingMode::Circular:
			TargetLocation = GetCircularFlyingLocation(InitialPosition);
		break;
		case EFlyingMode::AroundActor:
			TargetLocation = GetAroundActorLocation(Blackboard, StartLocation, SearchRadius);
		break;
		default:
			TargetLocation = StartLocation;
		break;
	}
	
	TargetLocation.Z = GetAltitudeAboveGround(TargetLocation);

	return TargetLocation;
}

bool UBTTask_FindFlyingTargetLocation::IsHittingSomething(AActor* ControlledPawn, FVector& StartLocation,FVector& EndLocation)
{
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(ControlledPawn);

	FHitResult HitResult;

	FCollisionShape SphereShape = FCollisionShape::MakeSphere(SphereRadius);

	FVector StartOffsetLocation = StartLocation + FVector(0, 0, 300.f);

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartOffsetLocation,
		EndLocation,
		FQuat::Identity,
		CollisionChannelIsHittingSomething,
		SphereShape,
		TraceParams
	);

	if (bDebug)
	{
		const int NumSteps = 10;
		for (int i = 0; i <= NumSteps; ++i)
		{
			float Alpha = (float)i / (float)NumSteps;
			FVector Pos = FMath::Lerp(StartLocation, EndLocation, Alpha);
			DrawDebugSphere(GetWorld(), Pos, SphereRadius, 12, FColor::Cyan, false, 5.0f);
		}

		DrawDebugLine(GetWorld(), StartLocation, EndLocation, bHit ? FColor::Red : FColor::Green, false, 5.0f, 0, 2.0f);

		if (bHit)
		{
			CurrentHitLocation = HitResult.ImpactPoint;
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, SphereRadius * 0.5f, 16, FColor::Red, false, 5.0f);
		}
	}

	return bHit;
}

FVector UBTTask_FindFlyingTargetLocation::GetRandomDirectionFromPreference(APawn* Enemy) const
{
	if (!Enemy) return FVector::ForwardVector;

	FRotator NewRot = Enemy->GetActorRotation();

	float YawOffset = 0.0f;

	switch (DirectionPreference)
	{
	case EDirectionPreference::Forward:
		YawOffset = FMath::RandRange(-20.f, 20.f);
		break;
	case EDirectionPreference::Backward:
		YawOffset = FMath::RandRange(160.f, 200.f);
		break;
	case EDirectionPreference::Left:
		YawOffset = FMath::RandRange(-110.f, -70.f);
		break;
	case EDirectionPreference::Right:
		YawOffset = FMath::RandRange(70.f, 110.f);
		break;
	case EDirectionPreference::Any:
	default:
		YawOffset = FMath::RandRange(-180.f, 180.f);
		break;
	}

	NewRot.Yaw += YawOffset;

	float PitchRange = 60.f;
	float VerticalMin = -PitchRange * (1 - VerticalBias);
	float VerticalMax = PitchRange * (1 + VerticalBias);

	NewRot.Pitch = FMath::Clamp(FMath::RandRange(VerticalMin, VerticalMax), -89.f, 89.f);

	return NewRot.Vector();
}

float UBTTask_FindFlyingTargetLocation::GetAltitudeAboveGround(const FVector& Location) const
{
	FHitResult Hit;
	FVector TraceStart = Location + FVector(0, 0, 1000.f);
	FVector TraceEnd = Location - FVector(0, 0, MaxAltitudeFromGround + 5000.f);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		TraceStart,
		TraceEnd,
		CollisionChannelIsHittingSomething
	);

	if (bDebug)
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, 2.0f);
		if (bHit)
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 30.f, 8, FColor::Green, false, 2.0f);
	}

	return bHit ? Hit.ImpactPoint.Z + FMath::FRandRange(MinAltitudeFromGround, MaxAltitudeFromGround) : Location.Z;
}

FVector UBTTask_FindFlyingTargetLocation::GetRandomFlyingLocation(const FVector& StartLocation,const FVector& InitialPosition, const FVector& Direction, float SearchRadius)
{
	FVector TargetLocation = StartLocation + (Direction * SearchRadius);
	if (!bCanExplore)
	{
		float DistanceFromSpawn = FVector::Dist(TargetLocation, InitialPosition);
		if (DistanceFromSpawn > SearchRadius + 350.0f)
		{
			TargetLocation = StartLocation + (-Direction * SearchRadius);
		}
	}
	return TargetLocation;
}

FVector UBTTask_FindFlyingTargetLocation::GetCircularFlyingLocation(const FVector& CenterRadius)
{
	FVector CircleCenter = CenterRadius;
	
	CurrentAngle = FMath::Fmod(CurrentAngle + CircleAngleStep, 360.f);
	float AngleRad = FMath::DegreesToRadians(CurrentAngle);

	FVector Offset;
	Offset.X = FMath::Cos(AngleRad) * FlyingCircleRadius;
	Offset.Y = FMath::Sin(AngleRad) * FlyingCircleRadius;
	
	return CircleCenter + Offset;
}

FVector UBTTask_FindFlyingTargetLocation::GetAroundActorLocation(UBlackboardComponent* Blackboard, const FVector& StartLocation, float SearchRadius)
{
	AActor* ReferenceActor = Cast<AActor>(Blackboard->GetValueAsObject(AroundActorKey.SelectedKeyName));
	if (!ReferenceActor)
	{
		return StartLocation;
	}

	FVector Center = ReferenceActor->GetActorLocation();

	float MinDist = Blackboard->GetValueAsFloat(MinDistanceFromActorKey.SelectedKeyName);
	float MaxDist = Blackboard->GetValueAsFloat(MaxDistanceFromActorKey.SelectedKeyName);
	MinDist = FMath::Clamp(MinDist, 0.f, MaxDist);
	
	float Distance = FMath::RandRange(MinDist, MaxDist);
	float RandomAngle = FMath::RandRange(0.f, 360.f);
	float AngleRad = FMath::DegreesToRadians(RandomAngle);

	FVector Offset;
	Offset.X = FMath::Cos(AngleRad) * Distance;
	Offset.Y = FMath::Sin(AngleRad) * Distance;

	FVector TargetLocation = Center + Offset;

	if (bDebug)
	{
		DrawDebugSphere(GetWorld(), Center, MinDist, 16, FColor::Orange, false, 1.5f);
		DrawDebugSphere(GetWorld(), Center, MaxDist, 16, FColor::Red, false, 1.5f);
		DrawDebugLine(GetWorld(), Center, TargetLocation, FColor::Orange, false, 1.5f);
	}

	return TargetLocation;
}
