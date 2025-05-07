// Fill out your copyright notice in the Description page of Project Settings.


#include "AITesting/AITestingController.h"

#include "AITesting/AITesting.h"
#include "BehaviorTree/BlackboardComponent.h"


AAITestingController::AAITestingController()
{
	PrimaryActorTick.bCanEverTick = true;
	CanSee = false;
	CanHear = false;
	CanTakeDamage = false;
}

void AAITestingController::BeginPlay()
{
	Super::BeginPlay();
	
	SetStateAsPatrolling();
}

void AAITestingController::InitializeBlackboardValues()
{
	Super::InitializeBlackboardValues();

	UNPCBaseStateEnemyDataAsset* DataAsset = Cast<UNPCBaseStateEnemyDataAsset>(AITesting->GetDataAsset());
	if (!IsValid(DataAsset))return;

	Blackboard->SetValueAsVector("InitialPosition", AITesting->GetActorLocation());
	Blackboard->SetValueAsVector("TargetLocation", FVector::ZeroVector);
	Blackboard->SetValueAsEnum("EntityState", (uint8)GetCurrentControllerEnemyState());
	Blackboard->SetValueAsFloat("SearchRadius", DataAsset->SearchRadius);
}

void AAITestingController::InitializeEnemyBase()
{
	Super::InitializeEnemyBase();
	AITesting = Cast<AAITesting>(GetPawn());
}

// Called every frame
void AAITestingController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

