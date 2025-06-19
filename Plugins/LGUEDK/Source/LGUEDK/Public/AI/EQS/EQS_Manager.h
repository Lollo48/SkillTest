// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EQS_Manager.generated.h"

UCLASS()
class LGUEDK_API AEQS_Manager : public AActor
{
	GENERATED_BODY()

public:
	AEQS_Manager();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void GeneratePoints();

private:
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= "WorldSubsystemSettings", meta = (AllowPrivateAccess = "true"))
	float GridRadius = 5000.0f;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= "WorldSubsystemSettings", meta = (AllowPrivateAccess = "true"))
	float StepSize = 200.0f;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= "WorldSubsystemSettings", meta = (AllowPrivateAccess = "true"))
	float ZStep = 1000.0f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= "WorldSubsystemSettings", meta = (AllowPrivateAccess = "true"))
	bool bWantsGeneratePoints = true;
};
