// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_RotateEntity.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Utility/LGDebug.h"

UBTService_RotateEntity::UBTService_RotateEntity(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Rotate Entity";
	
	ForceInstancing(true);
	
	bNotifyBecomeRelevant = true;
	bNotifyTick = true;
}

void UBTService_RotateEntity::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		return;
	}
	
	AttackTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
	ControlledPawn = Cast<AActor>(BlackboardComp->GetValueAsObject(ControlledPawnKey.SelectedKeyName));
}

void UBTService_RotateEntity::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	FVector LocationDifference = ControlledPawn->GetActorLocation() - AttackTarget->GetActorLocation();
	LocationDifference.Normalize();

	FVector ControlledPawnForwardVector = ControlledPawn->GetActorForwardVector();

	float Yaw = FVector::CrossProduct(LocationDifference,ControlledPawnForwardVector).Z * 100;

	FRotator CurrentRotation = ControlledPawn->GetActorRotation();

	FVector DirectionToTarget = (AttackTarget->GetActorLocation() - ControlledPawn->GetActorLocation()).GetSafeNormal();
	
	if (Yaw >= MaxYaw || Yaw <= MinYaw)
	{
		FRotator TargetRotation = DirectionToTarget.Rotation();
		
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, RotationSpeed);
		
		NewRotation.Pitch = CurrentRotation.Pitch;
		NewRotation.Roll = CurrentRotation.Roll;

		ControlledPawn->SetActorRotation(NewRotation);
	}
	
}
