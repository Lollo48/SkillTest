// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTestProject/Public/AI/DragonBoss.h"

#include "AI/BossController.h"


ADragonBoss::ADragonBoss()
{
}

void ADragonBoss::OnEnemyPassive()
{
	Super::OnEnemyPassive();
}

void ADragonBoss::OnEnemyPatrolling()
{
	Super::OnEnemyPatrolling();
	ABossController* AIController = Cast<ABossController>(GetController());
	if (!AIController)
	{
		return;
	};
	AIController->SetStateAsPatrolling();
}

void ADragonBoss::OnEnemyInvestigating()
{
	Super::OnEnemyInvestigating();
}

void ADragonBoss::OnEnemyAttack(AActor* InAttackTarget)
{
	Super::OnEnemyAttack(InAttackTarget);
}

void ADragonBoss::OnEnemyDead(AActor* InAttackTarget)
{
	Super::OnEnemyDead(InAttackTarget);
	
	ABossController* AIController = Cast<ABossController>(GetController());
	if (!AIController)
	{
		return;
	};
	
	AIController->SetStateAsDead(InAttackTarget);
}

void ADragonBoss::OnEnemyFlying()
{
	SetEnemyState(EEnemyState::Flying);
}

void ADragonBoss::BeginPlay()
{
	Super::BeginPlay();
	EnableEntity();
}

void ADragonBoss::Init()
{
	Super::Init();
}

void ADragonBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

