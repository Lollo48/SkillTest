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
		MovementComp = Character->GetCharacterMovement();
		if (!MovementComp)
		{
			FinishLatentTask(OwnerComp,EBTNodeResult::Failed);
			return EBTNodeResult::Failed; 
		}
		
		CharacterSpeed = MovementComp->MaxFlySpeed = MovementComp->MaxWalkSpeed;
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
	if (!Entity) return;
	
	FVector CurrentLocation = Entity->GetActorLocation();
	FVector ToTarget = TargetLocation - CurrentLocation;
	FVector Direction = ToTarget.GetSafeNormal();
	
	if (bDebug)
	{
		const FVector Start = CurrentLocation;
		const FVector End = Start + Direction * 10000.f; 

		DrawDebugLine(
			GetWorld(),
			Start,
			End,
			FColor::Red,
			false,
			5.f,
			0,
			3.0f 
		);
	}
	
	FRotator TargetRotation = Direction.Rotation();

	if (bDebug)
	{
		LGDebug::Log("TargetRotation: " + TargetRotation.ToString(), true);
	}
	
	Entity->SetActorRotation(TargetRotation);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
