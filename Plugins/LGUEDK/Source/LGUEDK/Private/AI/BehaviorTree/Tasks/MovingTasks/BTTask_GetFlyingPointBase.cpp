// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_GetFlyingPointBase.h"

#include "AI/NPC/NPCBase/NPCBaseController.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utility/LGDebug.h"

UBTTask_GetFlyingPointBase::UBTTask_GetFlyingPointBase(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Get Flying Point";
}

EBTNodeResult::Type UBTTask_GetFlyingPointBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ANPCBaseController* const Controller = Cast<ANPCBaseController>(OwnerComp.GetAIOwner());
	if (!Controller)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	APawn* const Enemy = Controller->GetPawn();
	if (!Enemy)
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
	
	FVector TargetLocation = FVector::ZeroVector;
	FVector ActorPosition = Enemy->GetActorLocation();
	
	for (int i = 0; i < 10; i++)
	{
		FRotator NewRotation = Enemy->GetActorRotation();
		NewRotation.Yaw += FMath::RandRange(-150.0f, 150.0f); // Z
		NewRotation.Pitch = FMath::RandRange(-100.0f, 100.0f); // Y
		FVector ForwardDirection = NewRotation.Vector();
		
		TargetLocation = GetTargetLocation(OwnerComp, Enemy,ForwardDirection,SearchRadius);
		
		if (!IsHittingSomething(Enemy, ActorPosition,TargetLocation))
		{
			FRotator CurrentLocation = (TargetLocation - ActorPosition).Rotation();
			CurrentLocation.Pitch = 0;
			Enemy->SetActorRotation(CurrentLocation);
			break;
		}
		
		TargetLocation = ActorPosition;
		
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(TargetLocationKey.SelectedKeyName, TargetLocation);

	
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;	
	
}

FVector UBTTask_GetFlyingPointBase::GetTargetLocation(UBehaviorTreeComponent& OwnerComp, APawn* AIPawn,FVector& Direction,float SearchRadius)
{
	if (!AIPawn) { return FVector::ZeroVector; }

	ANPCBaseStateEnemy* BaseStateEnemy = Cast<ANPCBaseStateEnemy>(AIPawn);
	if (!BaseStateEnemy)return FVector::ZeroVector;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return FVector::ZeroVector;

	FVector InitialPosition = Blackboard->GetValueAsVector(InitialPositionKey.SelectedKeyName);
	
	bCanExplore = BaseStateEnemy->GetDataAsset()->bWantExplore;
	
	FVector StartLocation = AIPawn->GetActorLocation();
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

bool UBTTask_GetFlyingPointBase::IsHittingSomething(AActor* ControlledPawn, FVector& StartLocation,FVector& EndLocation)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(ControlledPawn);

	// Risultato del Trace
	FHitResult HitResult;

	// Eseguiamo lo Sphere Trace
	bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		GetWorld(),
		StartLocation,
		EndLocation,
		SphereRadius,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true
	);


	if (bDebug)
	{

		DrawDebugSphere(GetWorld(), StartLocation, SphereRadius, 12, FColor::Blue, false, 5.0f);
		
		DrawDebugDirectionalArrow(GetWorld(), StartLocation, EndLocation, 100.0f, FColor::Cyan, false, 5.0f, 0, 2.0f);
		
		DrawDebugSphere(GetWorld(), EndLocation, SphereRadius, 12, FColor::Yellow, false, 5.0f);

		if (bHit)
		{
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, SphereRadius, 12, FColor::Red, false, 2.0f);
			DrawDebugLine(GetWorld(), StartLocation, HitResult.ImpactPoint, FColor::Red, false, 5.0f, 0, 2.0f);
		}
		else
		{
			DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, 5.0f, 0, 2.0f);
		}
	}

	return bHit;
}


