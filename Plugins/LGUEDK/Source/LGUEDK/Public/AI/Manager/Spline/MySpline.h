// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Enumerators/Spline/SplineMode.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MySpline.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInitSpline);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReachStartSplinePoint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReachEndSplinePoint);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReachSplinePoint);

UCLASS()
class LGUEDK_API AMySpline : public AActor
{
	GENERATED_BODY()

public:
	AMySpline();
	
	UPROPERTY(BlueprintCallable,BlueprintAssignable,Category = "State")
	FOnInitSpline OnInitSpline;
	UPROPERTY(BlueprintCallable,BlueprintAssignable,Category = "State")
	FOnReachStartSplinePoint OnReachStartSplinePoint;
	UPROPERTY(BlueprintCallable,BlueprintAssignable,Category = "State")
	FOnReachEndSplinePoint OnReachEndSplinePoint;
	UPROPERTY(BlueprintCallable,BlueprintAssignable,Category = "State")
	FOnReachSplinePoint OnReachSplinePoint;

	UFUNCTION(BlueprintCallable,Category = "Spline")
	void Initialize(AActor* InUsedActor = nullptr) { CurrentUser = InUsedActor; InitSpline(); }

	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "Spline")
	USplineComponent* GetSplineComponent() const { return SplineComponent; }

	UFUNCTION(BlueprintCallable,BlueprintPure,Category = "Spline")
	ESplineMode GetSplineMode() const { return SplineMode; }
	
	UFUNCTION(BlueprintCallable,Category = "Spline")
	virtual void InitSpline();

	UFUNCTION(BlueprintCallable,Category = "Spline")
	void ReachStartSplinePoint();
	
	UFUNCTION(BlueprintCallable,Category = "Spline")
	void ReachEndSplinePoint();

	UFUNCTION(BlueprintCallable,Category = "Spline")
	void ReachSplinePoint();

	UFUNCTION(BlueprintCallable,Category = "Spline")
	FVector GetRightPointAtSplineIndex();

	UFUNCTION(BlueprintCallable,Category = "Spline")
	void FollowSplinePath(float DeltaTime, float Speed);

	virtual void Tick(float DeltaSeconds) override;

protected:

	UPROPERTY(EditAnywhere,Category="Spline")
	USplineComponent* SplineComponent;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable,Category = "Spline")
	void CheckPointsDistance();

	UFUNCTION(BlueprintImplementableEvent,Category = "Spline")
	void InitSplineBP();

	UFUNCTION(BlueprintImplementableEvent,Category = "Spline")
	void ReachStartSplinePointBP();

	UFUNCTION(BlueprintImplementableEvent,Category = "Spline")
	void ReachEndSplinePointBP();

	UFUNCTION(BlueprintImplementableEvent,Category = "Spline")
	void ReachSplinePointBP();

private:
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "Spline Setting",meta = (AllowPrivateAccess = "true"))
	ESplineMode SplineMode;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "Spline Setting",meta = (AllowPrivateAccess = "true",EditCondition = "SplineMode == ESplineMode::SplineModeA"))
	float Tolerance = 600.0f;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "Spline Setting",meta = (AllowPrivateAccess = "true"))
	bool bPingPong = false;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "Spline Setting",meta = (AllowPrivateAccess = "true"))
	bool bWantsToStartFromBeginning = false;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "Spline Setting",meta = (AllowPrivateAccess = "true",EditCondition = "SplineMode == ESplineMode::SplineModeA"))
	bool bWantsContinueEvent = false;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "Spline Setting",meta = (AllowPrivateAccess = "true",EditCondition = "SplineMode == ESplineMode::SplineModeB"))
	bool bStopAtExtremes = false;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "Spline Setting",meta = (AllowPrivateAccess = "true",EditCondition = "SplineMode == ESplineMode::SplineModeB && bStopAtExtremes"))
	float StopDelay = 1.0f;

	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "Spline Setting",meta = (AllowPrivateAccess = "true",EditCondition = "SplineMode == ESplineMode::SplineModeB"))
	float OffsetZ = 390.f;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "Spline Setting",meta = (AllowPrivateAccess = "true",EditCondition = "SplineMode == ESplineMode::SplineModeB"))
	float RotationSpeed = 5.0f;

	UPROPERTY()
	AActor* CurrentUser;

	UPROPERTY()
	UCharacterMovementComponent* CharacterMovementComponent;

	UPROPERTY()
	FVector CurrentSplinePointLocation;
	
	UPROPERTY()
	int32 CurrentSplinePoints;
	
	UPROPERTY()
	int32 CurrentSplineIndex = 0;

	UPROPERTY()
	float SplineLength;

	UPROPERTY()
	float Direction;

	UPROPERTY()
	bool bMovingForward = true;
	UPROPERTY()
	bool bWasAtFirstPoint = false;
	UPROPERTY()
	bool bWasAtLastPoint = false;

	UPROPERTY()
	bool bIsWaitingAtPoint = true;

	UPROPERTY()
	float DistanceAlongSpline = 0.0f;
	FVector LastLocation = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "Spline Setting",meta = (AllowPrivateAccess = "true"))
	bool bDrawDebug;
	
	float StopTimer = 0.0f;

	UFUNCTION()
	void UpdateSplineIndex();

	UFUNCTION()
	void DrawDebugSplinePoints();
	

};
