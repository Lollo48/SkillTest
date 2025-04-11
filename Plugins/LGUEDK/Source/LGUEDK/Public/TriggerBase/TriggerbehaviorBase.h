// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TriggerbehaviorBase.generated.h"

/**
 * 
 */
UCLASS(Abstract,Blueprintable,BlueprintType,EditInlineNew)
class LGUEDK_API UTriggerbehaviorBase : public UObject
{
	GENERATED_BODY()
	
public:
	
	virtual void Enable(AActor* InTriggerObject) {EnableBP(InTriggerObject);};
	
	virtual void OnTriggering(float DeltaTime,AActor* InTriggerObject) {OnTriggeringBP(DeltaTime,InTriggerObject);};

	virtual void Disable(AActor* InTriggerObject) {DisableBP(InTriggerObject);};

protected:
	
	UFUNCTION(BlueprintImplementableEvent,Category="Triggerbehavior")
	void EnableBP(AActor* InTriggerObject);
	UFUNCTION(BlueprintImplementableEvent,Category="Triggerbehavior")
	void DisableBP(AActor* InTriggerObject);
	UFUNCTION(BlueprintImplementableEvent,Category="Triggerbehavior")
	void OnTriggeringBP(float DeltaTime,AActor* InTriggerObject);
};
