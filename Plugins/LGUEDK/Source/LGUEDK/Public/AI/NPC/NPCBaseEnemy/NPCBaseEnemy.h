// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/Enumerators/EMovementSpeed.h"
#include "AI/Enumerators/EnemyState.h"
#include "AI/NPC/NPCBase/NPCBase.h"
#include "AI/NPC/NPCData/NPCBaseEnemyDataAsset.h"
#include "NPCBaseEnemy.generated.h"

class ANPCPerceptionSystemController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FStatePassive);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FStatePatrolling);

UCLASS()
class LGUEDK_API ANPCBaseEnemy : public ANPCBase
{
	GENERATED_BODY()

public:
	
	ANPCBaseEnemy();

	UPROPERTY(BlueprintAssignable)
	FStatePassive OnStatePassive;

	UPROPERTY(BlueprintAssignable)
	FStatePatrolling OnStatePatrolling;

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "EntityPerceptionSystemController")
	ANPCPerceptionSystemController* GetPerceptionSystemController() const;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "On Enemy Passive"))
	virtual void OnEntityPassive();

	UFUNCTION(BlueprintCallable , meta = (DisplayName = "On Enemy Patrolling"))
	virtual void OnEntityPatrolling();

	UFUNCTION(BlueprintCallable , meta = (DisplayName = "Enemy State"))
	void SetMovementSpeed(EMovementSpeed MovementSpeed) const;

	UFUNCTION(BlueprintCallable , meta = (DisplayName = "Enemy State"))
	EEnemyState GetEntityState() const { return CurrentState; }
	UFUNCTION(BlueprintCallable , meta = (DisplayName = "Enemy State"))
	void SetEntityState(const EEnemyState NewState) { CurrentState = NewState; }

	virtual void EnableEntity() override { Super::EnableEntity(); };
	virtual void DisableEntity() override { Super::DisableEntity(); };
	
protected:
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "AI|Settings")
	UNPCBaseEnemyDataAsset* AIData;
	
	UPROPERTY(BlueprintReadOnly,Category = "AI|State")
	EEnemyState CurrentState = EEnemyState::Patrolling;
	
	virtual void BeginPlay() override;

	virtual void Init() override;
	
	UFUNCTION(BlueprintImplementableEvent,Category = "AI|State")
	void OnEntityPassiveBP();

	UFUNCTION(BlueprintImplementableEvent,Category = "AI|State")
	void OnEntityPatrollingBP();

private:

	UPROPERTY()
	ANPCPerceptionSystemController* PerceptionSystemController;
	
};
