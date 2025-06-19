// Copyright Villains, Inc. All Rights Reserved.


#include "AI/NPC/NPCBaseEnemy/NPCPerceptionSystemController.h"

#include "AI/Interfaces/AITargetInterface.h"
#include "AI/NPC/NPCBaseEnemy/NPCBaseEnemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISense_Hearing.h"
#include "Utility/LGDebug.h"


ANPCPerceptionSystemController::ANPCPerceptionSystemController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	//LGDebug::Log("ANPCPerceptionSystemController COSTRUTTORE",true);
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SetPerceptionComponent(*AIPerceptionComponent);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
}

ANPCBaseEnemy* ANPCPerceptionSystemController::GetBaseControlledEntity() const
{
	if (!IsValid(BaseControlledEntity))
	{
		UE_LOG(LogTemp, Error, TEXT("No controlled pawn"));
		return nullptr;
	}
	
	return BaseControlledEntity;
}

void ANPCPerceptionSystemController::SetStateAsPassive()
{
	SetIsEnable(false);
	SetStateAsPassiveBP();

	if (IsValid(BaseControlledEntity))
	{
		BaseControlledEntity->OnEntityPassive();
	}
}

void ANPCPerceptionSystemController::SetStateAsPatrolling()
{
	SetIsEnable(true);
	SetStateAsPatrollingBP();

	if (IsValid(BaseControlledEntity))
	{
		BaseControlledEntity->OnEntityPatrolling();
	}
}

void ANPCPerceptionSystemController::BeginPlay()
{
	Super::BeginPlay();
}

void ANPCPerceptionSystemController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!IsValid(AIPerceptionComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("PerceptionSystemControllerData invalid"));
		return;
	}
	
	SetUpPerceptionSystem();
	RegisterToPerceptionEvents();
}

void ANPCPerceptionSystemController::InitializeControlledEntity()
{
	BaseControlledEntity = Cast<ANPCBaseEnemy>(GetControlledEntity());
}

void ANPCPerceptionSystemController::InitializeBlackboardValues()
{
	Super::InitializeBlackboardValues();
}

void ANPCPerceptionSystemController::RegisterToPerceptionEvents()
{
	if (PerceptionSystemControllerData->CanSee)
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ANPCPerceptionSystemController::HandleSight);
	if (PerceptionSystemControllerData->CanHear)
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ANPCPerceptionSystemController::HandleHear);
	if (PerceptionSystemControllerData->CanTakeDamage)
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this,&ANPCPerceptionSystemController::HandleDamage);
}

void ANPCPerceptionSystemController::RemoveToPerceptionEvents()
{
	AIPerceptionComponent->OnTargetPerceptionUpdated.RemoveAll(this);
}

void ANPCPerceptionSystemController::SetUpPerceptionSystem()
{
	if (!IsValid(PerceptionSystemControllerData))
	{
		UE_LOG(LogTemp, Error, TEXT("PerceptionSystemControllerData invalid"));
		return;
	}
	
	if (PerceptionSystemControllerData->CanSee)
		SetUpSightConfig();
	if (PerceptionSystemControllerData->CanHear)
		SetUpHearingConfig();
	if (PerceptionSystemControllerData->CanTakeDamage)
		SetUpDamageConfig();
}

void ANPCPerceptionSystemController::SetUpSightConfig()
{
	if (SightConfig)
	{
		SightConfig->SightRadius =PerceptionSystemControllerData->SightRadius;
		SightConfig->LoseSightRadius = PerceptionSystemControllerData->LoseSightRadius;
		SightConfig->PeripheralVisionAngleDegrees = PerceptionSystemControllerData->PeripheralVisionAngleDegrees;
		SightConfig->AutoSuccessRangeFromLastSeenLocation = PerceptionSystemControllerData->AutoSuccessRangeFromLastSeenLocation;
		SightConfig->SetMaxAge(PerceptionSystemControllerData->SightMaxAge);
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		//LGDebug::Log("inizialize senso vista ",true);
		AIPerceptionComponent->ConfigureSense(*SightConfig);
		AIPerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());
	}
}

void ANPCPerceptionSystemController::SetUpHearingConfig()
{
	
	if (HearingConfig)
	{
		HearingConfig->HearingRange = PerceptionSystemControllerData->HearingRange;
		HearingConfig->SetMaxAge(PerceptionSystemControllerData->HearingMaxAge);
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
		
		AIPerceptionComponent->ConfigureSense(*HearingConfig);
	}
}

void ANPCPerceptionSystemController::SetUpDamageConfig()
{
	
	if (DamageConfig)
	{
		DamageConfig->SetMaxAge(PerceptionSystemControllerData->DamageMaxAge);
		AIPerceptionComponent->ConfigureSense(*DamageConfig);
	}
}


