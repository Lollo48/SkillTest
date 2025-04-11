// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TriggerBaseSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TriggerUtility.generated.h"

/**
 * 
 */
UCLASS()
class LGUEDK_API UTriggerUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static UTriggerBaseSubsystem* TriggerSubsystem;

	static void Init(UTriggerBaseSubsystem* InTriggerSubsystem);

	UFUNCTION(BlueprintPure)
	static UTriggerBaseSubsystem* GetTriggerBaseSubsystem(){return TriggerSubsystem;}

	UFUNCTION(BlueprintCallable)
	static void SetTriggerMap(ABaseTrigger* OutTriggers);

	UFUNCTION(BlueprintCallable)
	static TArray<ABaseTrigger*> GetTriggerContainer(FGameplayTag InGamePlayTag);

	UFUNCTION(BlueprintCallable)
	static void BindToOnEnable(const FGameplayTag& Name, const FEnableTrigger& Context, bool bUnique);

	UFUNCTION(BlueprintCallable)
	static void BindAllToOnEnable(const FGameplayTag& Name, const FEnableTrigger& Context, bool bUnique);

	UFUNCTION(BlueprintCallable)
	static void UnBindToOnEnable(const FGameplayTag& Name, const FEnableTrigger& Context);

	UFUNCTION(BlueprintCallable)
	static void UnBindAllToOnEnable(const FGameplayTag& Name, const FEnableTrigger& Context);

	UFUNCTION(BlueprintCallable)
	static void BindToOnDisable(const FGameplayTag& Name, const FDisableTrigger& Context, bool bUnique);

	UFUNCTION(BlueprintCallable)
	static void BindAllToOnDisable(const FGameplayTag& Name, const FDisableTrigger& Context, bool bUnique);

	UFUNCTION(BlueprintCallable)
	static void UnBindToOnDisable(const FGameplayTag& Name, const FDisableTrigger& Context);

	UFUNCTION(BlueprintCallable)
	static void UnBindAllToOnDisable(const FGameplayTag& Name, const FDisableTrigger& Context);
};
