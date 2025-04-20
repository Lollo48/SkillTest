// Copyright Villains, Inc. All Rights Reserved.


#include "BTTask_MoveFlyingEnemy.h"

#include <string>

#include "AIController.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utility/LGDebug.h"

UBTTask_MoveFlyingEnemy::UBTTask_MoveFlyingEnemy(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Move Flying Enemy";
	bNotifyTick = true;  
}

EBTNodeResult::Type UBTTask_MoveFlyingEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TargetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TargetLocationKey.SelectedKeyName);
	
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller)
	{
		FinishLatentTask(OwnerComp,EBTNodeResult::Failed);
		return EBTNodeResult::Failed; 
	}

	Entity = Controller->GetPawn();
	if (!Entity)
	{
		FinishLatentTask(OwnerComp,EBTNodeResult::Failed);
		return EBTNodeResult::Failed; 
	}

	Character = Cast<ACharacter>(Entity);
	if (Character)
	{
		MovementComp = Character->GetCharacterMovement();
		if (!MovementComp)
		{
			FinishLatentTask(OwnerComp,EBTNodeResult::Failed);
			return EBTNodeResult::Failed; 
		}
		
		CharacterSpeed = MovementComp->MaxFlySpeed = MovementComp->MaxWalkSpeed + IncrementMovementSpeed;
	}
	else
	{
		FinishLatentTask(OwnerComp,EBTNodeResult::Failed);
		return EBTNodeResult::Failed; 
	}
	
	return EBTNodeResult::InProgress; 
}

void UBTTask_MoveFlyingEnemy::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FVector CurrentLocation = Entity->GetActorLocation();
	FVector ToTarget = TargetLocation - CurrentLocation;
	FVector Direction = ToTarget.GetSafeNormal();
	FRotator TargetRotation = Direction.Rotation();
	
	FRotator DesiredRotation = TargetRotation + RotationOffset;
	FRotator CurrentActorRotation = Entity->GetActorRotation();
	
	FRotator SmoothedActorRotation = FMath::RInterpTo(CurrentActorRotation, DesiredRotation, DeltaSeconds, RotationSpeed);
	Entity->SetActorRotation(SmoothedActorRotation);
	
	FRotator CurrentMeshRotation = Character->GetMesh()->GetRelativeRotation();
	FRotator TargetMeshRotation = CurrentMeshRotation;
	TargetMeshRotation.Roll = -TargetRotation.Pitch;

	FRotator SmoothedMeshRotation = FMath::RInterpTo(CurrentMeshRotation, TargetMeshRotation, DeltaSeconds, RotationSpeed);
	Character->GetMesh()->SetRelativeRotation(SmoothedMeshRotation);
	
	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaSeconds, CharacterSpeed);
	Entity->SetActorLocation(NewLocation);
	
	if (bDebug)
	{
		DrawDebugLine(
			GetWorld(),
			CurrentLocation,
			TargetLocation,
			FColor::Red,
			false,
			0.1f,
			0,
			3.0f 
		);
	}
	
	bool bReachedLocation = FVector::Dist(NewLocation, TargetLocation) <= AcceptanceRadius;
	bool bMeshAligned = FMath::Abs(CurrentMeshRotation.Roll - TargetMeshRotation.Roll) <= RotationTolerance;
	bool bActorAligned = FMath::Abs(CurrentActorRotation.Yaw - DesiredRotation.Yaw) <= RotationTolerance;

	if (bReachedLocation && bActorAligned && bMeshAligned)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
