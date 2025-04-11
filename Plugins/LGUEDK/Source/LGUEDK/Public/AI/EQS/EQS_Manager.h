// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/Enumerators/EEnemyType.h"
#include "GameFramework/Actor.h"
#include "EQS_Manager.generated.h"

UCLASS()
class LGUEDK_API AEQS_Manager : public AActor
{
	GENERATED_BODY()

public:
	
	AEQS_Manager();
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	FVector GetAttackPoint(ANPCBaseStateEnemy* RequestActor,float const Margin);

	UFUNCTION(BlueprintCallable)
	FVector GetStrafePoint(ANPCBaseStateEnemy* RequestActor,float const Margin);
	
	UFUNCTION()
	bool GetCanSearchMeleePoint() const { return CanSearchMeleePoint; }
	UFUNCTION()
	void SetCanSearchMeleePoint(bool Value) { CanSearchMeleePoint = Value; }
	UFUNCTION()
	bool GetCanSearchRangedPoint() const { return CanSearchRengedPoint; }
	UFUNCTION()
	void SetCanSearchRangedPoint(bool Value) { CanSearchRengedPoint = Value; }
	UFUNCTION()
	bool GetCanSearchStrafingPoint() const { return CanSearchStrafingPoint; }
	UFUNCTION()
	void SetCanSearchStrafingPoint(bool Value) { CanSearchStrafingPoint = Value; }

protected:

	virtual void BeginPlay() override;
	
	TMap<EEnemyType,TArray<FVector>>Points;
	TArray<FVector> StrafePoints;
	
	UPROPERTY(EditAnywhere, Category = " AI|Melee Parameters")
	float MinAttackRadiusMelee = 400.0f;

	UPROPERTY(EditAnywhere, Category = "AI|Melee Parameters")
	float MaxAttackRadiusMelee = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "AI|Melee Parameters")
	float StepSizeMelee = 400.0f;
	
	UPROPERTY(EditAnywhere, Category = "AI|Ranged Parameters")
	float MinAttackRadiusRanged = 800.0f;

	UPROPERTY(EditAnywhere, Category = "AI|Ranged Parameters")
	float MaxAttackRadiusRanged = 2000.0f;

	UPROPERTY(EditAnywhere, Category = "AI|Ranged Parameters")
	float StepSizeRanged = 600.0f;
	
	UPROPERTY(EditAnywhere, Category = "AI|EQS Parameters")
	float UpdateMeleeInterval = 5.0f;
	
	UPROPERTY(EditAnywhere, Category = "AI|EQS Parameters")
	float UpdateRangedInterval = 5.0f;

	UPROPERTY(EditAnywhere, Category = "AI|Strafing Parameters")
	float StrafeGridRadius = 5000.0f;
	
	UPROPERTY(EditAnywhere, Category = "AI|Strafing Parameters")
	float StepSizePatrolling = 1000.0f;
	
	UPROPERTY(EditAnywhere, Category = "AI|Strafing Parameters")
	float MaxStrafeDistance = 4000.0f;
	
	UPROPERTY(EditAnywhere, Category = "AI|Strafing Parameters")
	float MinStrafeDistance = 2500.0f;

	UPROPERTY(EditAnywhere, Category = "AI|EQS Parameters")
	float UpdateStrafingInterval = 5.0f;
	
	void UpdateStrafeMeleePoints();
	void UpdateStrafeRangedPoints();
	
	
private:

	UPROPERTY()
	ACharacter* AttackTarget;
	
	float CurrentMeleeTime = 0;
	float CurrentRangedTime = 0;
	float CurrentStrafingTime = 0;

		
	UPROPERTY()
	bool CanSearchMeleePoint;

	UPROPERTY()
	bool CanSearchRengedPoint;

	UPROPERTY()
	bool CanSearchStrafingPoint;

	UFUNCTION(BlueprintCallable)
	void Initi();

	UFUNCTION(BlueprintCallable)
	bool IsHittingSomething(const FVector& Start, const FVector& End) const;

	UFUNCTION(BlueprintCallable)
	bool IsInRange(FVector& TargetPosition,FVector& CurrentPosition, const float& MinDistance, const float& MaxDistance);

	bool IsPointFree(const FVector& Point,const float Radius,const ANPCBaseStateEnemy* ControlledPawn) const;

	FVector TryGetPoint(TArray<FVector>& PossiblePoints, const ANPCBaseStateEnemy* RequestActor,float const Margin) const;
	
	TArray<FVector> SetGridPoints(TArray<FVector>& PossiblePoints,float const GridSize, float const StepSize,float const MinimusRadius, float const MaximusRadius);

	TArray<FVector> SetBaseGridPoints(float const GridRadius, float const StepSize);
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bShowDebug;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> CollisionChannel;
};



