// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/NPC/NPCBaseStateEnemy/NPCBaseStateEnemy.h"
#include "Components/SplineComponent.h"
#include "NPCBaseStateSplineEnemy.generated.h"

UCLASS()
class LGUEDK_API ANPCBaseStateSplineEnemy : public ANPCBaseStateEnemy
{
	GENERATED_BODY()

public:

	ANPCBaseStateSplineEnemy();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	FVector GetRightPointLocation();

protected:

	UPROPERTY()
	USplineComponent* SplineComponent;

	UPROPERTY()
	TArray<FVector> SplinePoints;

private:
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "AI|Spline Setting",meta = (AllowPrivateAccess = "true"))
	bool bPingPong = false;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "AI|Spline Setting",meta = (AllowPrivateAccess = "true"))
	bool bDrawDebug;

	UPROPERTY()
	int32 CurrentPointIndex = 0;

	UPROPERTY()
	int32 MaxIndex;

	UPROPERTY()
	bool bMovingForward = true;
	
	UPROPERTY()
	bool bWasAtFirstPoint = false;
	UPROPERTY()
	bool bWasAtLastPoint = false;

	UFUNCTION()
	void UpdatePointsIndex();

	UFUNCTION()
	void CheckPointsDistance();

	UFUNCTION()
	void DrawDebugSplinePoints();

};
