// Copyright Villains, Inc. All Rights Reserved.


#include "AI/Manager/WaveSystem/Utility/WaveHelper.h"

#include "AI/Manager/WaveSystem/Utility/WaveManagerUtility.h"


// Sets default values
AWaveHelper::AWaveHelper()
{
	
}


void AWaveHelper::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this,
		&AWaveHelper::InitWaveSystem,
		1.0f, 
		false 
	);
}

void AWaveHelper::InitWaveSystem()
{
	UWaveManagerUtility::SetCurrentWavesByTag(ArenaName);
	UWaveManagerUtility::TryStartWave();
	
	TimerHandle.Invalidate();
	InitWaveSystemBP();
}




