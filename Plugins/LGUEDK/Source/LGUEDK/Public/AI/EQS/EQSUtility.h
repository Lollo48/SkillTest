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

#pragma region Point Generation and Registration
	UFUNCTION(BlueprintCallable)
	static void GenerateGridPoints(const FVector& TargetLocation, float InGridRadius, float InStepSize,float ZStep);
	UFUNCTION(BlueprintCallable)
	static void RegisterEntity(AActor* InEntity);
	UFUNCTION(BlueprintCallable)
	static void UnregisterEntity(AActor* InEntity);
#pragma endregion
#pragma region Exploration - Generic
	UFUNCTION(BlueprintCallable)
	static TArray<FVector> GetPoints(FVector& TargetLocation, float InMinDistance, float InMaxDistance);
	UFUNCTION(BlueprintCallable)
	static FVector GetRandomExplorationPoint(const AActor* SelfEntity,const AActor* TargetIgnored, const FVector& InitialSpawnPosition, const FVector& CurrentPosition, float SearchRadius, bool bWantExplore);
	UFUNCTION(BlueprintCallable)
	static FVector GetBoidExplorationPoint(const AActor* SelfEntity, const FVector& InitialSpawnPosition,const FVector& CurrentPosition, float SearchRadius, bool bWantExplore);
	UFUNCTION(BlueprintCallable)
	static FVector GetAttackPoint(const AActor* SelfEntity,const AActor* TargetIgnored,const FVector& TargetLocation, float InMinDistance, float InMaxDistance,const float Margin);
#pragma endregion
#pragma region Exploration - Area Based
	UFUNCTION(BlueprintCallable)
	static void SetRegion(AEQS_Helper* InEQSHelper, const FGameplayTag& InTag);
	UFUNCTION(BlueprintCallable)
	static TArray<FVector> GetAreaPoints(const FVector& TargetLocation, float InMinDistance, float InMaxDistance, const FGameplayTag& InTag);
	UFUNCTION(BlueprintCallable)
	static FVector GetRandomExplorationAreaPoint(const AActor* SelfEntity,const AActor* TargetIgnored, const FVector& InitialSpawnPosition, const FVector& CurrentPosition, float SearchRadius, bool bWantExplore, const FGameplayTag& InTag);
	UFUNCTION(BlueprintCallable)
	static FVector GetBoidExplorationAreaPoint(const AActor* SelfEntity, const FVector& InitialSpawnPosition,const FVector& CurrentPosition, float SearchRadius, bool bWantExplore, const FGameplayTag& InTag);
	UFUNCTION(BlueprintCallable)
	static FVector GetAttackAreaPoint(const AActor* SelfEntity,const AActor* TargetIgnored,const FVector& TargetLocation, float InMinDistance, float InMaxDistance,const float Margin, const FGameplayTag& InTag);
#pragma endregion
#pragma region Chasing 
	UFUNCTION(BlueprintCallable)
	static FVector GetChasingAreaPoint(const AActor* SelfEntity,const AActor* TargetIgnored,const FVector& TargetLocation, float InMinDistance,float InMaxDistance);
	UFUNCTION(BlueprintCallable)
	static FVector GetChasingPoint(const AActor* SelfEntity,const AActor* TargetIgnored,const FVector& TargetLocation, float InMinDistance,float InMaxDistance);
#pragma endregion
};
