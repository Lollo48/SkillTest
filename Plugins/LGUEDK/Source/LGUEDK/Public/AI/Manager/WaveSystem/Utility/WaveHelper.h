// Copyright Villains, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AI/Manager/WaveSystem/Subsystem/WaveManagerSubsystem.h"
#include "GameFramework/Actor.h"
#include "WaveHelper.generated.h"

UCLASS()
class LGUEDK_API AWaveHelper : public AActor
{
	GENERATED_BODY()

public:

	AWaveHelper();

	FWaveClear OnWaveClear;

protected:

	virtual void BeginPlay() override;
	virtual void InitWaveSystem();
	UFUNCTION(BlueprintImplementableEvent,Category="Spawn Manager Base")
	void InitWaveSystemBP();

private:
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,category = WaveHelper, meta = (AllowPrivateAccess = "true"))
	bool bWantsToStartWave;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,category = WaveHelper, meta = (AllowPrivateAccess = "true"))
	bool bWantsToIncreaseWave;
	UPROPERTY(EditAnywhere, category = WaveHelper)
	FGameplayTag ArenaName;
	FTimerHandle TimerHandle;
};
