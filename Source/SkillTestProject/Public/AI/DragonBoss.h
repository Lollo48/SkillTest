// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"
#include "Data/BossDataAsset.h"
#include "DragonBoss.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FStateHit);

UCLASS()
class SKILLTESTPROJECT_API ADragonBoss : public ANPCBaseStateEnemy
{
	GENERATED_BODY()

public:
	ADragonBoss();

	UPROPERTY(BlueprintAssignable)
	FStateHit OnStateHit;

	virtual void OnEnemyPassive() override;
	virtual void OnEnemyPatrolling() override;
	virtual void OnEnemyInvestigating() override;
	virtual void OnEnemyAttack(AActor* InAttackTarget) override;
	virtual void OnEnemyDead(AActor* InAttackTarget) override;

protected:
	
	virtual void BeginPlay() override;
	
	virtual void Init() override;

	UPROPERTY()
	UBossDataAsset* BossDataAsset;

public:
	virtual void Tick(float DeltaTime) override;

};
