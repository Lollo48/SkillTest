// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"
#include "AI/NPC/NPCBaseStateSplineEnemy/NPCBaseStateSplineEnemy.h"
#include "Components/SplineComponent.h"
#include "Data/BossDataAsset.h"
#include "DragonBoss.generated.h"

UCLASS()
class SKILLTESTPROJECT_API ADragonBoss : public ANPCBaseStateSplineEnemy
{
	GENERATED_BODY()

public:
	
	ADragonBoss();

	virtual void OnEnemyPassive() override;
	virtual void OnEnemyPatrolling() override;
	virtual void OnEnemyInvestigating() override;
	virtual void OnEnemyAttack(AActor* InAttackTarget) override;
	virtual void OnEnemyDead(AActor* InAttackTarget) override;
	void OnEnemyFlying();

protected:
	
	virtual void BeginPlay() override;
	
	virtual void Init() override;

	UPROPERTY()
	UBossDataAsset* BossDataAsset;
	
public:
	virtual void Tick(float DeltaTime) override;

};
