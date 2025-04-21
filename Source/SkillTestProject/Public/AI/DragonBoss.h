// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Enumerators/ActionState/FlyingActionState.h"
#include "AI/NPC/NPCBaseStateSplineEnemy/NPCBaseStateSplineEnemy.h"
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
	
	UFUNCTION(BlueprintCallable,Category = "Spline")
	void ReachStartSplinePoint();
	
	UFUNCTION(BlueprintCallable,Category = "Spline")
	void ReachEndSplinePoint();

	UFUNCTION(BlueprintCallable,Category = "Spline")
	void ReachSplinePoint();

	UFUNCTION(BlueprintCallable,Category = "AI|FlyingState")
	EFlyingActionState GetFlyingActionState() const { return FlyingActionState; }

	UFUNCTION(BlueprintCallable,Category = "AI|FlyingState")
	void SetFlyingActionState(EFlyingActionState InFlyingActionState) { FlyingActionState = InFlyingActionState; }
	
protected:

	UPROPERTY()
	UBossDataAsset* BossDataAsset;

	UPROPERTY()
	EFlyingActionState FlyingActionState = EFlyingActionState::StartFlying;

	UPROPERTY()
	bool bWantsFly = true;
	
	virtual void BeginPlay() override;
	
	virtual void Init() override;

	UFUNCTION(BlueprintImplementableEvent,Category = "Spline")
	void ReachStartSplinePointBP();

	UFUNCTION(BlueprintImplementableEvent,Category = "Spline")
	void ReachEndSplinePointBP();

	UFUNCTION(BlueprintImplementableEvent,Category = "Spline")
	void ReachSplinePointBP();

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "Spline Setting",meta = (AllowPrivateAccess = "true"))
	float FlyingDuration = 10.f;

	UPROPERTY()
	float Timer;

	UFUNCTION()
	void SetMovementComponentActionMode(const EMovementMode Mode) const;

};
