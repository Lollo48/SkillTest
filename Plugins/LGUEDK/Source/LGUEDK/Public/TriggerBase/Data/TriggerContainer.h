// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerBase/BaseTrigger.h"
#include "TriggerContainer.generated.h"

USTRUCT(BlueprintType)
struct LGUEDK_API FTriggerContainer 
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Trigger")
	TArray<ABaseTrigger*> Triggers;

	FTriggerContainer(): Triggers(TArray<ABaseTrigger*>()) {}

};
