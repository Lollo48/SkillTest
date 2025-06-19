// Copyright Villains, Inc. All Rights Reserved.


#include "AI/NPC/NPCBase/NPCBaseController.h"

#include "AI/NPC/NPCBase/NPCBase.h"
#include "Components/AICrowdFollowingComponent/EnemyCrowdFollowingComponent.h"
#include "Utility/LGDebug.h"


ANPCBaseController::ANPCBaseController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UEnemyCrowdFollowingComponent>(TEXT("PhatFollowingComponent")))
{
	ControlledPawn = nullptr;
	bIsInitialized = false;
}

ANPCBase* ANPCBaseController::GetControlledEntity() const
{
	if (!IsValid(ControlledPawn))
	{
		UE_LOG(LogTemp, Error, TEXT("No controlled pawn"));
		return nullptr;
	}
	return ControlledPawn;
}

void ANPCBaseController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (GetIsInitialize())return;
	if (ANPCBase* const EnemyBase = Cast<ANPCBase>(InPawn))
	{
		ControlledPawn = EnemyBase;
		if (UBehaviorTree* const Tree = EnemyBase->GetBehaviorTree())
		{
			UBlackboardComponent* BlackboardComponent;
			UseBlackboard(Tree->BlackboardAsset,BlackboardComponent);
			Blackboard = BlackboardComponent;
			RunBehaviorTree(Tree);
			//LGDebug::Log("aic controller inizializzata",true);
		}
	}
	InitializeControlledEntity();
}

void ANPCBaseController::InitializeControllerAndBlackboard()
{
	InitializeController();
	
	CustomController();
	
	InitializeBlackboardValues();
	
	SetIsInitialize(true);
}

void ANPCBaseController::BeginPlay()
{
	//LGDebug::Log("BEGIN PLAY", true);
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimerForNextTick(this,&ANPCBaseController::InitializeControllerAndBlackboard);
}
