// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DragonBoss.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemyController.h"
#include "BossController.generated.h"

UCLASS()
class SKILLTESTPROJECT_API ABossController : public ANPCBaseStateEnemyController
{
	GENERATED_BODY()

public:
	ABossController();
	
	virtual void SetStateAsPassive() override;

	virtual void SetStateAsPatrolling() override;

	virtual void SetStateAsInvestigating() override;

	virtual void SetStateAsAttacking(AActor* InAttackTarget) override;

	virtual void SetStateAsDead(AActor* InAttackTarget) override;

protected:
	
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override ;
	
	virtual void InitializeBlackboardValues() override;

	virtual void HandleSight(AActor* Actor, FAIStimulus Stimulus) override ;
	
	virtual void OnLostSight() override ;

private:
	
	UPROPERTY()
	ADragonBoss* BossEntity;
};
