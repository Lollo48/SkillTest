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

void ADragonBoss::ReachStartSplinePoint()
{
	bWantsFly = FMath::RandBool();
	ReachStartSplinePointBP();
}

void ADragonBoss::ReachEndSplinePoint()
{
	if (!bWantsFly)return;
	
	ABossController* AIController = Cast<ABossController>(GetController());
	if (!AIController)
	{
		return;
	};
	SetEnemyState(EEnemyState::Flying);
	AIController->SetStateAsFlying();
	ReachEndSplinePointBP();
	bWantsFly = false;
}

void ADragonBoss::ReachSplinePoint()
{
	ReachSplinePointBP();
}

void ADragonBoss::BeginPlay()
{
	Super::BeginPlay();
	EnableEntity();

	MySpline->OnReachSplinePoint.AddDynamic(this, &ADragonBoss::ReachSplinePoint);
	MySpline->OnReachStartSplinePoint.AddDynamic(this, &ADragonBoss::ReachStartSplinePoint);
	MySpline->OnReachEndSplinePoint.AddDynamic(this, &ADragonBoss::ReachEndSplinePoint);
}

void ADragonBoss::Init()
{
	Super::Init();
}

void ADragonBoss::Tick(float DeltaTime)
{
	if (GetState()== EEnemyState::Flying)
		return;
	
	Super::Tick(DeltaTime);
}

