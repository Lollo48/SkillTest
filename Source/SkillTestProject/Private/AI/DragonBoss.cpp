// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTestProject/Public/AI/DragonBoss.h"

#include "AI/BossController.h"


ADragonBoss::ADragonBoss()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADragonBoss::OnEnemyPassive()
{
	Super::OnEnemyPassive();
}

void ADragonBoss::OnEnemyPatrolling()
{
	Super::OnEnemyPatrolling();
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
	GetMesh()->SetCollisionProfileName("Ragdoll", true);
	GetMesh()->SetSimulatePhysics(true);
}

void ADragonBoss::BeginPlay()
{
	Super::BeginPlay();
}

void ADragonBoss::Init()
{
	Super::Init();
}

void ADragonBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

