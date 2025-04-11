// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DetourCrowdAIController.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "NPCBaseController.generated.h"

class ANPCBase;

UCLASS()
class LGUEDK_API ANPCBaseController : public AAIController
{
	GENERATED_BODY()

public:
	
	 ANPCBaseController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(Blueprintable,BlueprintCallable)
	ANPCBase* GetControlledPawn() const {return ControlledPawn;}
	
protected:

	UPROPERTY()
	ANPCBase* ControlledPawn;
	
	UFUNCTION()
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	virtual void InitializeBlackboardValues() {};

	UFUNCTION()
	virtual void CustomController() {};
	
	virtual void BeginPlay() override;
	
};


