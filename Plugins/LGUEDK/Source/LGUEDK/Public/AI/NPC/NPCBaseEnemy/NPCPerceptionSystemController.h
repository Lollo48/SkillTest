// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NPCBaseEnemy.h"
#include "AI/NPC/NPCBase/NPCBaseController.h"
#include "AI/NPC/NPCData/PerceptionSystemControllerData.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Utility/LGDebug.h"
#include "NPCPerceptionSystemController.generated.h"


UCLASS()
class LGUEDK_API ANPCPerceptionSystemController : public ANPCBaseController
{
	GENERATED_BODY()

public:
	
	explicit ANPCPerceptionSystemController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(Blueprintable,BlueprintCallable,blueprintPure)
	ANPCBaseEnemy* GetBaseControlledEntity() const ;
	
	UFUNCTION(BlueprintCallable, Category = "State")
	virtual void SetStateAsPassive();
	
	UFUNCTION(BlueprintCallable, Category = "State")
	virtual void SetStateAsPatrolling();

protected:

	UPROPERTY(EditAnywhere,Category = "PerceptionSystemControllerData")
	UPerceptionSystemControllerData* PerceptionSystemControllerData;
	
	FTimerHandle LostSightTimerHandle;
	
	FTimerHandle LostHearTimerHandle;

	FTimerHandle LostDamageTimerHandle;
	
	virtual void BeginPlay() override;
	
	virtual void OnPossess(APawn* InPawn) override;

	virtual void InitializeControlledEntity() override;
	
	virtual void InitializeBlackboardValues() override;

	virtual void InitializeController() override {Super::InitializeController();}

	virtual void CustomController() override{Super::CustomController();};
	
	UFUNCTION()
	virtual void HandleSight(AActor* Actor, FAIStimulus Stimulus) {if (!GetIsEnable())return; };
	UFUNCTION()
	virtual void HandleHear(AActor* Actor, FAIStimulus Stimulus) {if (!GetIsEnable())return; };
	UFUNCTION()
	virtual void HandleDamage(AActor* Actor, FAIStimulus Stimulus) {if (!GetIsEnable())return; };
	
	UFUNCTION()
	virtual void OnLostSight() {};
	UFUNCTION()
	virtual void OnLostHear() {};
	UFUNCTION()
	virtual void OnLostDamage() {};

	UFUNCTION()
	virtual void RegisterToPerceptionEvents();
	UFUNCTION()
	virtual void RemoveToPerceptionEvents();

	UPROPERTY()
	UAIPerceptionComponent* AIPerceptionComponent;
	
	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;
	
	UPROPERTY()
	UAISenseConfig_Hearing* HearingConfig;

	UPROPERTY()
	UAISenseConfig_Damage* DamageConfig;
	
	void SetUpPerceptionSystem();
	void SetUpSightConfig();
	void SetUpHearingConfig();
	void SetUpDamageConfig();

	UFUNCTION(BlueprintImplementableEvent,Category = "AI|State")
	void SetStateAsPassiveBP();

	UFUNCTION(BlueprintImplementableEvent,Category = "AI|State")
	void SetStateAsPatrollingBP();

private:
	
	UPROPERTY()
	ANPCBaseEnemy* BaseControlledEntity;

	
	
};


