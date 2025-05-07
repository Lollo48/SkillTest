// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NPCBaseEnemy.h"
#include "AI/NPC/NPCBase/NPCBaseController.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Utility/LGDebug.h"
#include "NPCPerceptionSystemController.generated.h"

class ANPCBaseEnemy;

UCLASS()
class LGUEDK_API ANPCPerceptionSystemController : public ANPCBaseController
{
	GENERATED_BODY()

public:
	
	explicit ANPCPerceptionSystemController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "State")
	virtual void SetStateAsPassive();
	
	UFUNCTION(BlueprintCallable, Category = "State")
	virtual void SetStateAsPatrolling();

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sense")
	bool CanSee;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sense")
	bool CanHear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sense")
	bool CanTakeDamage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight", meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition = "CanSee"))
	float SightRadius = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight", meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition = "CanSee"))
	float LoseSightRadius = 2500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight", meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "180.0", UIMax = "180.0", EditCondition = "CanSee"))
	float PeripheralVisionAngleDegrees = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight", meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition = "CanSee"))
	float AutoSuccessRangeFromLastSeenLocation = 520.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Sight", meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition = "CanSee"))
	float SightMaxAge = 5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Hearing", meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition = "CanHear"))
	float HearingRange = 3000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Hearing", meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition = "CanHear"))
	float HearingMaxAge = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Damage", meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition = "CanTakeDamage"))
	float DamageMaxAge = 5.0f;
	
	FTimerHandle LostSightTimerHandle;
	
	FTimerHandle LostHearTimerHandle;

	FTimerHandle LostDamageTimerHandle;
	
	virtual void BeginPlay() override;
	
	virtual void OnPossess(APawn* InPawn) override;

	virtual void InitializeEnemyBase() override {Super::InitializeEnemyBase();} 
	
	virtual void InitializeBlackboardValues() override;

	virtual void InitializeController() override {Super::InitializeController();}

	virtual void CustomController() override{Super::CustomController();};
	
	UFUNCTION()
	virtual void HandleSight(AActor* Actor, FAIStimulus Stimulus) {if (!bIsEnabled)return; };
	UFUNCTION()
	virtual void HandleHear(AActor* Actor, FAIStimulus Stimulus) {if (!bIsEnabled)return; };
	UFUNCTION()
	virtual void HandleDamage(AActor* Actor, FAIStimulus Stimulus) {if (!bIsEnabled)return; };
	
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
	
};


