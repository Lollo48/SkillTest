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

	if (ACharacter* Character = Cast<ACharacter>(Entity))
	{
		if (UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement())
		{
			MovementComp->MaxFlySpeed = MovementComp->MaxWalkSpeed + IncrementMovementSpeed;
			MovementComp->BrakingDecelerationFlying = BrakingDecelerationFlying;
			MovementComp->bOrientRotationToMovement = bOrientRotationToMovement;
			MovementComp->bUseControllerDesiredRotation = bUseControllerDesiredRotation;
		}
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
	
	Entity->AddMovementInput(ToTarget);
	
	float Distance = FVector::Distance(CurrentLocation, TargetLocation);
	
	if (Distance < AcceptanceRadius)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	
	// bool bCloseEnough = ToTarget.Size() <= AcceptanceRadius * 3.0f; 
	//
	// if (!bCloseEnough)
	// {
	// 	FVector Direction = ToTarget.GetSafeNormal();
	// 	FRotator TargetRotation = Direction.Rotation();
	// 	DesiredRotation = TargetRotation + RotationOffset; 
	// }
	//
	// FRotator CurrentActorRotation = Entity->GetActorRotation();
	// FRotator SmoothedActorRotation = FMath::RInterpTo(CurrentActorRotation, DesiredRotation, DeltaSeconds, RotationSpeed);
	// Entity->SetActorRotation(SmoothedActorRotation);
	//
	// // Mesh Rotation
	// FRotator CurrentMeshRotation = Character->GetMesh()->GetRelativeRotation();
	// FRotator TargetMeshRotation = CurrentMeshRotation;
	// TargetMeshRotation.Pitch = -DesiredRotation.Pitch; 
	//
	// FRotator SmoothedMeshRotation = FMath::RInterpTo(CurrentMeshRotation, TargetMeshRotation, DeltaSeconds, RotationSpeed);
	// Character->GetMesh()->SetRelativeRotation(SmoothedMeshRotation);
	//
	// // Movement
	// FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaSeconds, CharacterSpeed);
	// Entity->SetActorLocation(NewLocation);
	//
	// // Debug
	// if (bDebug)
	// {
	// 	DrawDebugLine(GetWorld(), CurrentLocation, TargetLocation, FColor::Red, false, 0.1f, 0, 3.0f);
	// 	LGDebug::Log("CurrentMeshRotation" + CurrentMeshRotation.ToString(), true);
	// }
	//
	// // Checks
	// bool bReachedLocation = FVector::Dist(NewLocation, TargetLocation) <= AcceptanceRadius;
	// bool bMeshAligned = FMath::Abs(CurrentMeshRotation.Roll - TargetMeshRotation.Roll) <= RotationTolerance;
	// bool bActorAligned = FMath::Abs(CurrentActorRotation.Yaw - DesiredRotation.Yaw) <= RotationTolerance;

	// if (bReachedLocation && bActorAligned && bMeshAligned)
	// {
	// 	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	// }
}
