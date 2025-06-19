// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PerceptionSystemControllerData.generated.h"

/**
 */
UCLASS()
class LGUEDK_API UPerceptionSystemControllerData : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sense")
	bool CanSee;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sense")
	bool CanHear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sense")
	bool CanTakeDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight", meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition = "CanSee",EditConditionHides))
	float SightRadius = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight", meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition = "CanSee",EditConditionHides))
	float LoseSightRadius = 2500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight", meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "180.0", UIMax = "180.0", EditCondition = "CanSee",EditConditionHides))
	float PeripheralVisionAngleDegrees = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight", meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition = "CanSee",EditConditionHides))
	float AutoSuccessRangeFromLastSeenLocation = 520.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight", meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition = "CanSee",EditConditionHides))
	float SightMaxAge = 5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Hearing", meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition = "CanHear",EditConditionHides))
	float HearingRange = 3000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Hearing", meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition = "CanHear",EditConditionHides))
	float HearingMaxAge = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Damage", meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition = "CanTakeDamage",EditConditionHides))
	float DamageMaxAge = 5.0f;

	
};
