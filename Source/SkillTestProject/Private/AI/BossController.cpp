﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTestProject/Public/AI/BossController.h"

#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"


// Sets default values
ABossController::ABossController()
{
	CanHear = true;
	CanSee = true;
	CanTakeDamage = false;
	SetUpPerceptionSystem();
	BossEntity = nullptr;
}

void ABossController::SetStateAsPassive()
{
	Super::SetStateAsPassive();
	
	if (!Blackboard)return;
	Blackboard->SetValueAsEnum(TEXT("EnemyState"), uint8(EEnemyState::Passive));

	if (!BossEntity)return;
	BossEntity->OnEnemyPassive();
}

void ABossController::SetStateAsPatrolling()
{
	Super::SetStateAsPatrolling();
	
	if (!Blackboard)return;
	Blackboard->SetValueAsEnum(TEXT("EnemyState"), uint8(EEnemyState::Patrolling));

	if (!BossEntity)return;
	BossEntity->OnEnemyPatrolling();
}

void ABossController::SetStateAsInvestigating()
{
	Super::SetStateAsInvestigating();
	
	if (!Blackboard)return;
	Blackboard->SetValueAsEnum(TEXT("EnemyState"), uint8(EEnemyState::Investigating));

	if (!BossEntity)return;
	BossEntity->OnEnemyInvestigating();
}

void ABossController::SetStateAsAttacking(AActor* InAttackTarget)
{
	Super::SetStateAsAttacking(InAttackTarget);
	
	if (!Blackboard)return;
	Blackboard->SetValueAsEnum(TEXT("EnemyState"), uint8(EEnemyState::Attacking));

	if (!BossEntity)return;
	BossEntity->OnEnemyAttack(InAttackTarget);
}

void ABossController::SetStateAsDead(AActor* InAttackTarget)
{
	Super::SetStateAsDead(InAttackTarget);
	if (!Blackboard)return;
	Blackboard->SetValueAsEnum(TEXT("EnemyState"), uint8(EEnemyState::Dead));

}

void ABossController::SetStateAsFlying()
{
	if (!Blackboard)return;
	Blackboard->SetValueAsEnum(TEXT("EnemyState"), uint8(EEnemyState::Flying));

	if (!BossEntity)return;
	BossEntity->OnEnemyFlying();
}

void ABossController::UpdateBBMovementActionState(EMovementActionState InMovementActionState)
{
	if (!Blackboard)return;
	Blackboard->SetValueAsEnum(TEXT("MovementActionState"), uint8(InMovementActionState));
}

void ABossController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABossController::BeginPlay()
{
	Super::BeginPlay();
	SetStateAsPatrolling();
}

void ABossController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ABossController::InitializeBlackboardValues()
{
	Super::InitializeBlackboardValues();

	if (!Blackboard)return;
	if (!BossEntity)return;
	
	UBossDataAsset* EnemyDataAsset =Cast<UBossDataAsset>(BossEntity->GetDataAsset()); 
	if (!EnemyDataAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("BossEntity->GetDataAsset() returned nullptr!"));
		return;
	}

	Blackboard->SetValueAsEnum("EnemyState",uint8(EEnemyState::Patrolling));
	Blackboard->SetValueAsEnum("MovementActionState",uint8(EMovementActionState::Walking));
	Blackboard->SetValueAsVector("InitialPosition",BossEntity->GetActorLocation());
	Blackboard->SetValueAsFloat(TEXT("SearchRadius"), EnemyDataAsset->SearchRadius);
	Blackboard->SetValueAsFloat(TEXT("TimeBeforeNextStep"), EnemyDataAsset->TimeBeforeNextStep);
	Blackboard->SetValueAsVector(TEXT("TargetLocation"), FVector::ZeroVector);
	Blackboard->SetValueAsObject(TEXT("AttackTarget"), nullptr);
	Blackboard->SetValueAsFloat(TEXT("MaxAttackRadius"), EnemyDataAsset->MaxAttackRadius);
	Blackboard->SetValueAsFloat(TEXT("MinAttackRadius"), EnemyDataAsset->MinAttackRadius);
}

void ABossController::InitializeController()
{
	Super::InitializeController();
	BossEntity = Cast<ADragonBoss>(GetControlledPawn());
}

void ABossController::HandleSight(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor)return;
	if (!BossEntity)return;
	
	Super::HandleSight(Actor, Stimulus);
	
	if (Stimulus.WasSuccessfullySensed())
	{
		if (Stimulus.Type != UAISense::GetSenseID<UAISense_Sight>())return;
		
		if (GetWorld()->GetTimerManager().IsTimerActive(LostSightTimerHandle))
		{
			GetWorld()->GetTimerManager().ClearTimer(LostSightTimerHandle);
			//LGDebug::Log("TIMER PERSO ANNULLATO", true);
		}

		if (BossEntity->GetState() == EEnemyState::Patrolling || BossEntity->GetState() == EEnemyState::Passive)
		{
			SetStateAsAttacking(Actor);
		}
	}
	else
	{
		if (BossEntity->GetState() == EEnemyState::Chasing || BossEntity->GetState() == EEnemyState::Attacking)
		{
			GetWorld()->GetTimerManager().SetTimer(
				LostSightTimerHandle,
				this,
				&ABossController::OnLostSight,
				SightMaxAge,
				false
			);
			//LGDebug::Log("LOST SIGHT PLAYER", true);
		}
		// ClearFocus(EAIFocusPriority::Gameplay);
	}		
	
}

void ABossController::OnLostSight()
{
	Super::OnLostSight();
	SetStateAsInvestigating();
}


