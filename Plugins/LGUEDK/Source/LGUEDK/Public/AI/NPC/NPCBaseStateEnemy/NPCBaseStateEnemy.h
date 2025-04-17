// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/Enumerators/ActionState/MovementActionState.h"
#include "AI/NPC/NPCBaseEnemy/NPCBaseEnemy.h"
#include "AI/NPC/NPCData/NPCBaseStateEnemyDataAsset.h"
#include "NPCBaseStateEnemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(
	FStateInvestigating);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FStateChasing , AActor*, Target);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FStatePending , AActor*, Target);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FStateAttacking , AActor*, Target);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FStateDead, AActor*, Target);

UCLASS()
class LGUEDK_API ANPCBaseStateEnemy : public ANPCBaseEnemy
{
	GENERATED_BODY()

public:

	ANPCBaseStateEnemy();

	UPROPERTY(BlueprintAssignable)
	FStateInvestigating OnStateInvestigating;

	UPROPERTY(BlueprintAssignable)
	FStateChasing OnStateChasing;

	UPROPERTY(BlueprintAssignable)
	FStatePending OnStatePending;
	
	UPROPERTY(BlueprintAssignable)
	FStateAttacking OnStateAttacking;
	
	UPROPERTY(BlueprintAssignable)
	FStateDead OnStateDead;

	virtual void OnEnemyPassive() override;

	virtual void OnEnemyPatrolling() override;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "On Enemy Investigating"))
	virtual void OnEnemyInvestigating();
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "On Enemy Chasing"))
	virtual void OnEnemyChasing(AActor* InAttackTarget);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "On Enemy Pending"))
	virtual void OnEnemyPending(AActor* InAttackTarget);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "On Enemy Attack"))
	virtual void OnEnemyAttack(AActor* InAttackTarget);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "On Enemy Dead"))
	virtual void OnEnemyDead(AActor* InAttackTarget);
	
	UFUNCTION(BlueprintCallable,BlueprintPure)
	UNPCBaseStateEnemyDataAsset* GetDataAsset() const;

	UFUNCTION(BlueprintCallable)
	EEnemyType GetEnemyType() const { return NPCEnemyStateDataAsset->MyEnemyType; }

	UFUNCTION(BlueprintCallable,Category = "AI|FlyingState")
	EMovementActionState GetMovementActionState() const { return MovementActionState; }

	UFUNCTION(BlueprintCallable,Category = "AI|FlyingState")
	void SetMovementActionState(EMovementActionState InMovementActionState) { MovementActionState = InMovementActionState; }
	
	UFUNCTION(BlueprintCallable)
	void SetAttackTarget(AActor* Target) { AttackTarget = Target; }
	
	UFUNCTION(BlueprintPure)
	AActor* GetAttackTarget() const { return AttackTarget; }
	
protected:
	
	UPROPERTY()
	AActor* AttackTarget;
	
	UPROPERTY()
	UNPCBaseStateEnemyDataAsset* NPCEnemyStateDataAsset;

	UPROPERTY()
	EMovementActionState MovementActionState = EMovementActionState::Walking;

	virtual void BeginPlay() override;

	virtual void Init() override;

	virtual void EnableEntity() override { Super::EnableEntity(); };
	virtual void DisableEntity() override { Super::DisableEntity(); };

	UFUNCTION(BlueprintImplementableEvent,Category = "AI|State")
	void OnEnemyInvestigatingBP();
	UFUNCTION(BlueprintImplementableEvent,Category = "AI|State")
	void OnEnemyChasingBP(AActor* InAttackTarget);
	UFUNCTION(BlueprintImplementableEvent,Category = "AI|State")
	void OnEnemyPendingBP(AActor* InAttackTarget);
	UFUNCTION(BlueprintImplementableEvent,Category = "AI|State")
	void OnEnemyAttackBP(AActor* InAttackTarget);
	UFUNCTION(BlueprintImplementableEvent,Category = "AI|State")
	void OnEnemyDeadBP(AActor* InAttackTarget);
	
};

