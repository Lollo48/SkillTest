// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EQS_WorldSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EQSUtility.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UEQSUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static UEQS_WorldSubsystem* EQSWorldSubsystem;

	static void InitWorldSubsystem(UEQS_WorldSubsystem* InEQSWorldSubsystem);
	
	UFUNCTION(BlueprintCallable)
	static UEQS_WorldSubsystem* GetEQSWorldSubsystem(){ return EQSWorldSubsystem; }
	
	UFUNCTION(BlueprintCallable)
	static TArray<FVector> GenerateGridPoints(FVector& TargetLocation, float InGridRadius, float InStepSize);
	
	UFUNCTION(BlueprintCallable)
	static void RegisterEntity(AActor* Requester,AActor* Target);
	
	UFUNCTION(BlueprintCallable)
	static void UnregisterEntity(AActor* Requester);
	
	UFUNCTION(BlueprintCallable)
	static TArray<FVector> TryGetPoint(AActor* Requester,AActor* Target, float InMinDistance, float InMaxDistance);

	UFUNCTION(BlueprintCallable)
	static void RemovePoint(AActor* Target,const FVector& PointToRemove);
};
