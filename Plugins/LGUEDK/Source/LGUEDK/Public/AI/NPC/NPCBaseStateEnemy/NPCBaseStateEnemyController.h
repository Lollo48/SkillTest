// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NPCBaseStateEnemy.h"
#include "AI/NPC/NPCBaseEnemy/NPCPerceptionSystemController.h"
#include "NPCBaseStateEnemyController.generated.h"

class ANPCBaseStateEnemy;

UCLASS()
class LGUEDK_API ANPCBaseStateEnemyController : public ANPCPerceptionSystemController
{
	GENERATED_BODY()

public:
	
	ANPCBaseStateEnemyController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void SetStateAsPassive() override;

	virtual void SetStateAsPatrolling() override;

	UFUNCTION(BlueprintCallable, Category = "State")
	virtual void SetStateAsInvestigating();

	UFUNCTION(BlueprintCallable, Category = "State")
	virtual void SetStateAsChasing(AActor* InAttackTarget) ;
	
	UFUNCTION(BlueprintCallable, Category = "State")
	virtual void SetStateAsAttacking(AActor* InAttackTarget) ;

	UFUNCTION(BlueprintCallable, Category = "State")
	virtual void SetStateAsDead(AActor* InAttackTarget);

	UFUNCTION(BlueprintCallable, Category = "State")
	virtual void SetInitialState(EEnemyState State){}

	UFUNCTION(BlueprintCallable, Category = "State")
	EEnemyState GetCurrentControllerEnemyState() { return CurrentState; }

	UFUNCTION(BlueprintCallable, Category = "State")
	void SetCurrentControllerEnemyState(EEnemyState InState) { CurrentState = InState; }

protected:
	
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override { Super::OnPossess(InPawn); };
	
	virtual void InitializeBlackboardValues() override { Super::InitializeBlackboardValues(); };

	virtual void CustomController() override{Super::CustomController();};
	
	virtual void HandleSight(AActor* Actor, FAIStimulus Stimulus) override { Super::HandleSight(Actor, Stimulus); };
	virtual void HandleHear(AActor* Actor, FAIStimulus Stimulus) override { Super::HandleHear(Actor, Stimulus); };
	virtual void HandleDamage(AActor* Actor, FAIStimulus Stimulus) override { Super::HandleDamage(Actor, Stimulus); };
	
	virtual void OnLostSight() override { Super::OnLostSight(); };
	virtual void OnLostHear() override { Super::OnLostHear(); };
	virtual void OnLostDamage() override { Super::OnLostDamage(); };

	UFUNCTION(BlueprintImplementableEvent,Category = "AI|State")
	void SetStateAsInvestigatingBP();
	UFUNCTION(BlueprintImplementableEvent,Category = "AI|State")
	void SetStateAsChasingBP(AActor* InAttackTarget);
	UFUNCTION(BlueprintImplementableEvent,Category = "AI|State")
	void SetStateAsAttackingBP(AActor* InAttackTarget);
	UFUNCTION(BlueprintImplementableEvent,Category = "AI|State")
	void SetStateAsDeadBP(AActor* InAttackTarget);

private:
	UPROPERTY()
	EEnemyState CurrentState;
};
