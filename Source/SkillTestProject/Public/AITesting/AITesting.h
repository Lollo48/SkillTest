// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"
#include "AITesting.generated.h"

UCLASS()
class SKILLTESTPROJECT_API AAITesting : public ANPCBaseStateEnemy
{
	GENERATED_BODY()

public:
	
	AAITesting();

protected:
	
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
