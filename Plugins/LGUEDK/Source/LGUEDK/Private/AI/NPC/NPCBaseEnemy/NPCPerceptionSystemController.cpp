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

void ANPCPerceptionSystemController::SetStateAsPassive()
{
	bIsEnabled = false;
	RemoveToPerceptionEvents();
	AIPerceptionComponent->Deactivate();
	AIPerceptionComponent->SetActive(false);
	AIPerceptionComponent->SetComponentTickEnabled(false);
	SetActorTickEnabled(false);
	SetStateAsPassiveBP();
}

void ANPCPerceptionSystemController::SetStateAsPatrolling()
{
	bIsEnabled = true;
	RegisterToPerceptionEvents();
	AIPerceptionComponent->Activate();
	AIPerceptionComponent->SetActive(true);
	AIPerceptionComponent->SetComponentTickEnabled(true);
	SetActorTickEnabled(true);
	SetStateAsPatrollingBP();
}

void ANPCPerceptionSystemController::BeginPlay()
{
	Super::BeginPlay();
}

void ANPCPerceptionSystemController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	SetUpPerceptionSystem();
}

void ANPCPerceptionSystemController::InitializeBlackboardValues()
{
	Super::InitializeBlackboardValues();
}

void ANPCPerceptionSystemController::RegisterToPerceptionEvents()
{
	if (CanSee)
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ANPCPerceptionSystemController::HandleSight);
	if (CanHear)
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ANPCPerceptionSystemController::HandleHear);
	if (CanTakeDamage)
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this,&ANPCPerceptionSystemController::HandleDamage);
}

void ANPCPerceptionSystemController::RemoveToPerceptionEvents()
{
	AIPerceptionComponent->OnTargetPerceptionUpdated.RemoveAll(this);
}

void ANPCPerceptionSystemController::SetUpPerceptionSystem()
{
	if (CanSee)
		SetUpSightConfig();
	if (CanHear)
		SetUpHearingConfig();
	if (CanTakeDamage)
		SetUpDamageConfig();
}

void ANPCPerceptionSystemController::SetUpSightConfig()
{
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
		AIPerceptionComponent->ConfigureSense(*SightConfig);
		AIPerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());
	}
}

void ANPCPerceptionSystemController::SetUpHearingConfig()
{
	
	if (HearingConfig)
	{
		HearingConfig->HearingRange = HearingRange;
		HearingConfig->SetMaxAge(HearingMaxAge);
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
		DamageConfig->SetMaxAge(DamageMaxAge);
		AIPerceptionComponent->ConfigureSense(*DamageConfig);
	}
}


