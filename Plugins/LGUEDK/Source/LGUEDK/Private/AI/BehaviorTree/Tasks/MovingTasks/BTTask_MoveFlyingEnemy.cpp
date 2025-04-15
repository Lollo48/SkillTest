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

	APawn* Enemy = Controller->GetPawn();
	if (!Enemy)
	{
		FinishLatentTask(OwnerComp,EBTNodeResult::Failed);
		return EBTNodeResult::Failed; 
	}
	
	if (ACharacter* Character = Cast<ACharacter>(Enemy))
	{
		UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();
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
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return;

	APawn* Enemy = Controller->GetPawn();
	if (!Enemy) return;
	
	FVector CurrentLocation = Enemy->GetActorLocation();
	
	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation,TargetLocation,DeltaSeconds,CharacterSpeed);
	
	Enemy->SetActorLocation(NewLocation);
	
	if (FVector::Dist(NewLocation, TargetLocation) <= AcceptanceRadius)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
