// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DetourCrowdAIController.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "NPCBaseController.generated.h"

UCLASS()
class LGUEDK_API ANPCBaseController : public AAIController
{
	GENERATED_BODY()

public:
	
	explicit ANPCBaseController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(Blueprintable,BlueprintCallable,blueprintPure)
	ANPCBase* GetControlledEntity() const;
	
	UFUNCTION(Blueprintable,BlueprintCallable,blueprintPure)
	bool GetIsEnable() const {return bIsEnabled;}
	
	UFUNCTION(Blueprintable,BlueprintCallable,blueprintPure)
	bool GetIsInitialize() const {return bIsInitialized;}
	
protected:
	
	UFUNCTION()
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void InitializeControllerAndBlackboard();

	UFUNCTION()
	virtual void InitializeBlackboardValues() {	if (!Blackboard)return;};
	
	UFUNCTION()
	virtual void InitializeControlledEntity(){};

	UFUNCTION()
	virtual void CustomController() {};
	
	virtual void BeginPlay() override;

	virtual void InitializeController(){}
	
	UFUNCTION(Blueprintable,BlueprintCallable)
	void SetIsInitialize(bool bInit) const {bInit = bIsInitialized;}
	
	UFUNCTION(Blueprintable,BlueprintCallable)
	void SetIsEnable(bool bEnable) const {bEnable = bIsEnabled;}

private:
	
	UPROPERTY()
	ANPCBase* ControlledPawn;

	UPROPERTY()
	bool bIsInitialized;

	UPROPERTY()
	bool bIsEnabled = false;
	
};


