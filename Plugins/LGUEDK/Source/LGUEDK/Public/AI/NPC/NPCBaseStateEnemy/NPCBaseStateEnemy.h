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

	virtual void OnEntityPassive() override;

	virtual void OnEntityPatrolling() override;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "On Enemy Investigating"))
	virtual void OnEntityInvestigating();
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "On Enemy Chasing"))
	virtual void OnEntityChasing(AActor* InTarget);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "On Enemy Pending"))
	virtual void OnEntityPending(AActor* InTarget);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "On Enemy Attack"))
	virtual void OnEntityAttack(AActor* InTarget);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "On Enemy Dead"))
	virtual void OnEntityDead(AActor* InSelf);
	
	UFUNCTION(BlueprintCallable,BlueprintPure)
	UNPCBaseStateEnemyDataAsset* GetDataAsset() const;

	UFUNCTION(BlueprintCallable)
	EEnemyType GetEnemyType() const { return NPCEnemyStateDataAsset->MyEnemyType; }

	UFUNCTION(BlueprintCallable,Category = "AI|FlyingState")
	EMovementActionState GetMovementActionState() const { return MovementActionState; }

	UFUNCTION(BlueprintCallable,Category = "AI|FlyingState")
	void SetMovementActionState(EMovementActionState InMovementActionState) { MovementActionState = InMovementActionState; }
	
	UFUNCTION(BlueprintCallable)
	void SetTarget(AActor* InTarget) { Target = InTarget; }
	
	UFUNCTION(BlueprintPure)
	AActor* GetTarget() const { return Target; }
	
	virtual void EnableEntity() override { Super::EnableEntity(); };
	virtual void DisableEntity() override { Super::DisableEntity(); };
	
protected:
	
	UPROPERTY()
	AActor* Target;
	
	UPROPERTY()
	UNPCBaseStateEnemyDataAsset* NPCEnemyStateDataAsset;

	UPROPERTY()
	EMovementActionState MovementActionState = EMovementActionState::Walking;

	virtual void BeginPlay() override;

	virtual void Init() override;

	UFUNCTION(BlueprintImplementableEvent,Category = "AI|State")
	void OnEntityInvestigatingBP();
	UFUNCTION(BlueprintImplementableEvent,Category = "AI|State")
	void OnEntityChasingBP(AActor* InAttackTarget);
	UFUNCTION(BlueprintImplementableEvent,Category = "AI|State")
	void OnEntityPendingBP(AActor* InAttackTarget);
	UFUNCTION(BlueprintImplementableEvent,Category = "AI|State")
	void OnEntityAttackBP(AActor* InAttackTarget);
	UFUNCTION(BlueprintImplementableEvent,Category = "AI|State")
	void OnEntityDeadBP(AActor* InAttackTarget);
	
};

