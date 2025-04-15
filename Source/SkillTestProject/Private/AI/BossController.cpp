// Fill out your copyright notice in the Description page of Project Settings.


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
	if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
		BlackboardComp->SetValueAsEnum(TEXT("EnemyState"), uint8(EEnemyState::Passive));

	if (BossEntity != nullptr)
		BossEntity->OnEnemyPassive();
}

void ABossController::SetStateAsPatrolling()
{
	Super::SetStateAsPatrolling();
	if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
		BlackboardComp->SetValueAsEnum(TEXT("EnemyState"), uint8(EEnemyState::Patrolling));

	if (BossEntity != nullptr)
		BossEntity->OnEnemyPatrolling();
}

void ABossController::SetStateAsInvestigating()
{
	Super::SetStateAsInvestigating();
	if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
		BlackboardComp->SetValueAsEnum(TEXT("EnemyState"), uint8(EEnemyState::Investigating));

	if (BossEntity != nullptr)
		BossEntity->OnEnemyInvestigating();
}

void ABossController::SetStateAsAttacking(AActor* InAttackTarget)
{
	Super::SetStateAsAttacking(InAttackTarget);
	if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
		BlackboardComp->SetValueAsEnum(TEXT("EnemyState"), uint8(EEnemyState::Attacking));

	if (BossEntity != nullptr)
		BossEntity->OnEnemyAttack(InAttackTarget);
}

void ABossController::SetStateAsDead(AActor* InAttackTarget)
{
	Super::SetStateAsDead(InAttackTarget);
	if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
		BlackboardComp->SetValueAsEnum(TEXT("EnemyState"), uint8(EEnemyState::Attacking));

	if (BossEntity != nullptr)
		BossEntity->OnEnemyAttack(InAttackTarget);
}

void ABossController::SetStateAsFlying()
{
	if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
		BlackboardComp->SetValueAsEnum(TEXT("EnemyState"), uint8(EEnemyState::Flying));

	if (BossEntity != nullptr)
		BossEntity->OnEnemyFlying();
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


