// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTestProject/Public/AI/DragonBoss.h"

#include <string>

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
	ReachEndSplinePoint();
	Timer = 100.0f;
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
	SetMovementComponentActionMode(MOVE_Walking);

	Timer = FlyingDuration;
	
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

	SetMovementComponentActionMode(MOVE_Flying);

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

UAttackDataAsset* ADragonBoss::GetAttackDataAsset()
{
	float Distance = FVector::Dist(GetActorLocation(), GetAttackTarget()->GetActorLocation());

	UAttackDataAsset* BestAttack = nullptr;
	int32 HighestPriority = -1;

	if (AttacksCombo.Num() == 1)
	{
		SetCurrentAttackCombo(AttacksCombo[0]);
		return AttacksCombo[0];
	}

	for (UAttackDataAsset* Attack : AttacksCombo)
	{
		if (!Attack || !Attack->bCanPerformAttack || Attack->AttackType == EComboAttackState::ComboAttack1)
			continue;

		if (Distance >= Attack->MinRange && Distance <= Attack->MaxRange)
		{
			if (Attack->Priority > HighestPriority)
			{
				BestAttack = Attack;
				HighestPriority = Attack->Priority;
			}
		}
	}

	if (BestAttack)
	{
		BestAttack->bCanPerformAttack = false;

		SetCurrentAttackCombo(BestAttack);
		
		AttackCooldownMap.Add(BestAttack, BestAttack->Cooldown);

		if (!AttacksCombo.Contains(BestAttack))return nullptr;
		AttacksCombo.Remove(BestAttack);
		
		return BestAttack;
	}

	SetCurrentAttackCombo(AttacksCombo[0]);
	
	return  AttacksCombo[0];
}

void ADragonBoss::UpdateCooldownAttacksCombo(float DeltaTime)
{
	TArray<UAttackDataAsset*> FinishedCooldowns;

	for (auto& Elem : AttackCooldownMap)
	{
		UAttackDataAsset* Attack = Elem.Key;
		float& TimeRemaining = Elem.Value;

		TimeRemaining -= DeltaTime;
		if (TimeRemaining <= 0.f)
		{
			Attack->bCanPerformAttack = true;

			if (AttackCooldownMap.Contains(Attack))
			{
				AttackCooldownMap.Remove(Attack);
			}
			
			if (!AttacksCombo.Contains(Attack))
			{
				AttacksCombo.Add(Attack);
			}
		}
	}
}

void ADragonBoss::InitAttacksCombo()
{
	for (auto Element : AttacksCombo)
	{
		Element->bCanPerformAttack = true;
	}
	SetCurrentAttackCombo(AttacksCombo[0]);
}

void ADragonBoss::BeginPlay()
{
	Super::BeginPlay();
	EnableEntity();

	MySpline->OnReachSplinePoint.AddDynamic(this, &ADragonBoss::ReachSplinePoint);
	MySpline->OnReachStartSplinePoint.AddDynamic(this, &ADragonBoss::ReachStartSplinePoint);
	MySpline->OnReachEndSplinePoint.AddDynamic(this, &ADragonBoss::ReachEndSplinePoint);

	bWantsFly = true;

	InitAttacksCombo();
}

void ADragonBoss::Init()
{
	Super::Init();
}

void ADragonBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AttackCooldownMap.Num() > 0)
	{
		UpdateCooldownAttacksCombo(DeltaTime);
	}

	if (GetMovementActionState() == EMovementActionState::Flying)
	{
		DistanceFromGround = GetAltitudeAboveGround();
		//LGDebug::Log(FString::Printf(TEXT("Distance: %.2f"), DistanceFromGround), true);
		if (Timer <= 0.f)
		{
			//LGDebug::Log("Flying",true);
			SetFlyingActionState(EFlyingActionState::EndFlying);
			SetMovementComponentActionMode(MOVE_Walking);
		}
		Timer -= DeltaTime;
	}
}

void ADragonBoss::SetMovementComponentActionMode(const EMovementMode Mode) const
{
	UCharacterMovementComponent* MovementComp = this->GetCharacterMovement();
	if (!MovementComp)return;
	MovementComp->SetMovementMode(Mode);
}

float ADragonBoss::GetAltitudeAboveGround()
{
	FHitResult Hit;
	FVector TraceStart = GetActorLocation() - OffsetPivot;
	FVector TraceEnd = TraceStart - FVector(0, 0, 40000.f);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		TraceStart,
		TraceEnd,
		CollisionChannelIsHittingSomething
	);

	if (bDebug)
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, .1f);
		if (bHit)
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 30.f, 8, FColor::Green, false, .1f);

		//LGDebug::Log("Hit.ImpactPoint.Z: " + Hit.Location.ToString(), true);
	}
	
	return bHit ? (GetActorLocation().Z - Hit.Location.Z) : GetActorLocation().Z;
}

