// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Enumerators/ActionState/FlyingActionState.h"
#include "AI/NPC/AttacksData/AttackDataAsset.h"
#include "AI/NPC/NPCBaseStateSplineEnemy/NPCBaseStateSplineEnemy.h"
#include "Data/BossDataAsset.h"
#include "DragonBoss.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStartAttack,UAttackDataAsset*, InAttackCombo );

UCLASS()
class SKILLTESTPROJECT_API ADragonBoss : public ANPCBaseStateSplineEnemy
{
	GENERATED_BODY()

public:
	
	ADragonBoss();

	UPROPERTY(BlueprintAssignable)
	FStartAttack OnAttackStart;

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

	UFUNCTION(BlueprintCallable,Category = "AI")
	float GetDistanceFromGround() const { return DistanceFromGround; }

	UFUNCTION(BlueprintCallable,Category = "AI")
	UAttackDataAsset* GetAttackDataAsset();

	UFUNCTION(BlueprintCallable,Category = "AI")
	void SetCurrentAttackCombo(UAttackDataAsset* InAttackCombo) {CurrentAttackCombo = InAttackCombo;}

	UFUNCTION(BlueprintCallable,Category = "AI")
	void PerformAttackCombo() {OnAttackStart.Broadcast(CurrentAttackCombo);}
	
protected:

	UPROPERTY()
	UBossDataAsset* BossDataAsset;

	UPROPERTY()
	EFlyingActionState FlyingActionState = EFlyingActionState::StartFlying;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "AI",meta = (AllowPrivateAccess = "true"))
	TArray<UAttackDataAsset*> AttacksCombo;

	UPROPERTY()
	UAttackDataAsset* CurrentAttackCombo;

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
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "AI",meta = (AllowPrivateAccess = "true"))
	float FlyingDuration = 10.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI", meta=(AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionChannel> CollisionChannelIsHittingSomething;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI", meta=(AllowPrivateAccess = "true"))
	FVector OffsetPivot;

	UPROPERTY()
	float DistanceFromGround = 0.f;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "AI",meta = (AllowPrivateAccess = "true"))
	bool bDebug;

	UPROPERTY()
	float Timer;

	UPROPERTY()
	TMap<UAttackDataAsset*, float> AttackCooldownMap;

	UFUNCTION()
	void InitAttacksCombo();

	UFUNCTION()
	void UpdateCooldownAttacksCombo(float DeltaTime);

	UFUNCTION()
	void SetMovementComponentActionMode(const EMovementMode Mode) const;

	UFUNCTION()
	float GetAltitudeAboveGround();

};


