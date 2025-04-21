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
	SetMovementActionState(EMovementActionState::Walking);
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
	ReachStartSplinePointBP();
	UCharacterMovementComponent* MovementComp = this->GetCharacterMovement();
	if (!MovementComp)return;
	MovementComp->SetMovementMode(MOVE_Walking);
	
	if (!bWantsFly)return;
	bWantsFly = true;

	if (MySpline->GetSplineMode()==ESplineMode::SplineModeB)
	{
		MySpline->SetCanPerformSpline(true);
	}
}

void ADragonBoss::ReachEndSplinePoint()
{
	ReachEndSplinePointBP();
	
	if (!bWantsFly)return;
	
	ABossController* AIController = Cast<ABossController>(GetController());
	if (!AIController)
	{
		return;
	};
	AIController->UpdateBBMovementActionState(EMovementActionState::Flying);
	SetMovementActionState(EMovementActionState::Flying);
	bWantsFly = false;
	
	UCharacterMovementComponent* MovementComp = this->GetCharacterMovement();
	if (!MovementComp)return;
	MovementComp->SetMovementMode(MOVE_Flying);

	Timer = FlyingDuration;

	if (MySpline->GetSplineMode()==ESplineMode::SplineModeB)
	{
		MySpline->SetCanPerformSpline(false);
	}
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

	bWantsFly = true;
}

void ADragonBoss::Init()
{
	Super::Init();
}

void ADragonBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetMovementActionState() == EMovementActionState::Flying)
	{
		//LGDebug::Log("Timer " + FString::SanitizeFloat(Timer), true);
		if (Timer <= 0.f)
		{
			//LGDebug::Log("Flying",true);
			SetFlyingActionState(EFlyingActionState::EndFlying);
			ReachStartSplinePoint();
		}
		Timer -= DeltaTime;
	}
}

