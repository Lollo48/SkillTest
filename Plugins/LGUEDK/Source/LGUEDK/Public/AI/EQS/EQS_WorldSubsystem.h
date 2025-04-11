// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"
#include "Data/EQSContainer.h"
#include "Subsystems/WorldSubsystem.h"
#include "EQS_WorldSubsystem.generated.h"

/**
 * 
 */
UCLASS(Abstract,Blueprintable,BlueprintType)
class LGUEDK_API UEQS_WorldSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
	virtual void Tick(float DeltaTime) override;

public:
	
	TArray<FVector> GenerateGridPoints(FVector& TargetLocation, float InGridRadius, float InStepSize);
	void RegisterEntity(AActor* Requester,AActor* Target);
	void UnregisterEntity(AActor* Requester);
	TArray<FVector> TryGetPoint(AActor* Requester,AActor* Target, float InMinDistance, float InMaxDistance);
	void RemovePoint(AActor* Target,const FVector& PointToRemove);

	virtual TStatId GetStatId() const override;
	
protected:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;

	virtual void Init();

	UFUNCTION(BlueprintImplementableEvent,Category="Triggerbehavior")
	void InitBP();

private:
	
	void UpdatePoints();
	TArray<FVector> GetPoint(AActor* Requester,TArray<FVector>& PossiblePoints,AActor* Target, float InMinDistance, float InMaxDistance) ;
	
	bool IsInRange(FVector& TargetPosition,FVector& CurrentPosition, const float InMinDistance, const float InMaxDistance);

	UPROPERTY(EditDefaultsOnly, Category = "Trigger")
	TArray<TSubclassOf<AGameModeBase>>GameModes;
	
	UPROPERTY()
	TArray<AActor*> ActiveEntity;
	UPROPERTY()
	TMap<AActor*,FEQSContainer> Points;

	UPROPERTY(EditDefaultsOnly,Category="WorldSubsystem Debug")
	float GridSize;

	UPROPERTY(EditDefaultsOnly,Category="WorldSubsystem Debug")
	float StepSize;
	
	UPROPERTY(EditDefaultsOnly,Category="WorldSubsystem Debug")
	float UpdateInterval = 5.0f;
	
	UPROPERTY()
	float CurrentUpdateTime = 0.0f;
	UPROPERTY()
	bool bIsUpdating;
	
	UPROPERTY(EditDefaultsOnly,Category="WorldSubsystem Debug")
	bool bShowDebug = false;
};
