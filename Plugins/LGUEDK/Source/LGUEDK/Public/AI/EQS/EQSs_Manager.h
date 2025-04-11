// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EQSs_Manager.generated.h"

UCLASS()
class LGUEDK_API AEQSs_Manager : public AActor
{
	GENERATED_BODY()

public:
	
	AEQSs_Manager();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Blueprintable,BlueprintCallable)
	TArray<FVector> TryGetPossiblePoints(AActor* MyAttackTarget);

	UFUNCTION(Blueprintable,BlueprintCallable)
	void RemoveUsablePoints(AActor* MyAttackTarget,FVector& Point);

	UFUNCTION(Blueprintable,BlueprintCallable)
	void ClearMap(AActor* MyAttackTarget);
	
protected:
	
	TMap<AActor*,TArray<FVector>> CurrentPoints;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "AI|Parameters")
	float GridRadius = 3000.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "AI|Parameters")
	float StepSize = 400.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "AI|Parameters")
	float TimeToUpdatePoints;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "AI|Parameters")
	bool bShowDebug;
	
	UFUNCTION(Blueprintable,BlueprintCallable)
	void SetBaseGridPoints(AActor* ActorGridOriginPosition);

private:
	
	bool bNeedToUpdatePoints;

	float CurrentUpdateTime;

	UFUNCTION(Blueprintable,BlueprintCallable)
	void SetPossiblePoints(AActor* MyAttackTarget,TArray<FVector>& Points);
	
	UFUNCTION(Blueprintable,BlueprintCallable)
	void UpdatePoints();
};

