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
}

void ANPCPerceptionSystemController::SetStateAsPassive()
{
	SetStateAsPassiveBP();
}

void ANPCPerceptionSystemController::SetStateAsPatrolling()
{
	SetStateAsPatrollingBP();
}

void ANPCPerceptionSystemController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANPCPerceptionSystemController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ANPCPerceptionSystemController::InitializeBlackboardValues()
{
	Super::InitializeBlackboardValues();
}


void ANPCPerceptionSystemController::SetUpPerceptionSystem()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SetPerceptionComponent(*AIPerceptionComponent);

	if (CanSee)
		SetUpSightConfig();
	if (CanHear)
		SetUpHearingConfig();
	if (CanTakeDamage)
		SetUpDamageConfig();
}

void ANPCPerceptionSystemController::SetUpSightConfig()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		SightConfig->SightRadius = SightRadius;
		SightConfig->LoseSightRadius = LoseSightRadius;
		SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionAngleDegrees;
		SightConfig->AutoSuccessRangeFromLastSeenLocation = AutoSuccessRangeFromLastSeenLocation;
		SightConfig->SetMaxAge(SightMaxAge);
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		//LGDebug::Log("inizialize senso vista ",true);
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ANPCPerceptionSystemController::HandleSight);
		AIPerceptionComponent->ConfigureSense(*SightConfig);
		AIPerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());
	}
}

void ANPCPerceptionSystemController::SetUpHearingConfig()
{
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	if (HearingConfig)
	{
		HearingConfig->HearingRange = HearingRange;
		HearingConfig->SetMaxAge(HearingMaxAge);
		HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ANPCPerceptionSystemController::HandleHear);
		AIPerceptionComponent->ConfigureSense(*HearingConfig);
	}
}

void ANPCPerceptionSystemController::SetUpDamageConfig()
{
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Config"));
	if (DamageConfig)
	{
		DamageConfig->SetMaxAge(DamageMaxAge);

		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this,&ANPCPerceptionSystemController::HandleDamage);
		AIPerceptionComponent->ConfigureSense(*DamageConfig);
	}
}


