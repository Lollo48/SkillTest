// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AITesting.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemyController.h"
#include "AITestingController.generated.h"

UCLASS()
class SKILLTESTPROJECT_API AAITestingController : public ANPCBaseStateEnemyController
{
	GENERATED_BODY()

public:
	AAITestingController();

protected:
	
	virtual void BeginPlay() override;
	virtual void InitializeBlackboardValues() override;
	virtual void InitializeEnemyBase() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	AAITesting* AITesting;
};
