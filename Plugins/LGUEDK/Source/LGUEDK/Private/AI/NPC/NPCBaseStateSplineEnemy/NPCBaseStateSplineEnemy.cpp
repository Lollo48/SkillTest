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
	MySpline->Initialize(this);
}

void ANPCBaseStateSplineEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AMySpline* ANPCBaseStateSplineEnemy::GetMySpline() const
{
	if (MySpline == nullptr)return nullptr;
	
		return MySpline;
}


