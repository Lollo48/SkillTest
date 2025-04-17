#include "AI/NPC/NPCBaseStateSplineEnemy/NPCBaseStateSplineEnemy.h"
#include "DrawDebugHelpers.h"
#include "Utility/LGDebug.h"

ANPCBaseStateSplineEnemy::ANPCBaseStateSplineEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ANPCBaseStateSplineEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (MySpline == nullptr)return;
	MySpline->Initialize(this);
}

void ANPCBaseStateSplineEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (MySpline == nullptr)return;

	if (MySpline->GetSplineMode() == ESplineMode::SplineModeB)
		MySpline->FollowSplinePath(DeltaTime,GetDataAsset()->WalkSpeed);
}

AMySpline* ANPCBaseStateSplineEnemy::GetMySpline() const
{
	if (MySpline == nullptr)return nullptr;
	
	return MySpline;
}


